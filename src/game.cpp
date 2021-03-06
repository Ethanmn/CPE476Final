#include "game.h"
#include "graphics/mesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include <graphics/material.h>
#include <iostream>
#include "frustum.h"
#include "timer.h"

#include "graphics/texture.h"

const int MAX_NUM_BUTTERFLY_SYSTEMS = 10; //For optimization just in case
const float COLOR_RATIO_MIN = 0.0;
const float COLOR_LOSS_RATE = 0.3;
const float MAX_FLOWER_TIME = 10000.0f;

namespace {
   bool showTreeShadows = false;
   bool draw_collision_box = false;
   bool switchBlinnPhongShading = false;
   bool eatFlower = false;
   bool deerInWater = false;
   bool displayTitleScreen = true;

   bool isFadingFlowers = true;
   float fadingTimeBeforeWater = -1.0f;

   int lighting = 0;
   int raining = 0;

   float countLightning = 0.0;
   int numLightning = 0;

   enum GameMode {
      START,
      PLAY,
   };
   GameMode current_mode = START;
   Timer draw_timer;
   bool show_fps = false;
   enum class AdjustType {
      FOV,
      NEAR,
      FAR,
      CAM_HEIGHT,
      CAM_DIST,
      HEIGHT_MAP
   };
   AdjustType adjust_mode = AdjustType::FAR;
   SpringMotion<float> far_plane_spring(
         0.128f, // strength
         0.00065f, // damp
         0.01f) // speed
         ;
   boost::optional<float> far_plane_target;
}

Game::Game() :
   attribute_location_map_({draw_shader_.getShaders().getAttributeLocationMap()}),
   ground_(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::GROUND))),
   deer_(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::DEER_WALK)),
         Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::DEER_EAT)),
         Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::DEER_SLEEP)),
         Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::DEER_POUNCE)),
         Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::LEAF)),
         glm::vec3()),
   day_night_boxes_(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::TIME_STONE)), ground_),
   treeGen(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::TREE)),
            Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::LEAF)),
            ground_),
   bushGen(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::BUSH)),
         ground_),
   rockGen(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::ROCK)),
         ground_),

   /* temporary solution to two flower meshes and textures */
   daisyGen(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::DAISY)),
            Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::EATEN_DAISY)), TextureType::DAISY,
            ground_, -0.1f, -1.6f),
   roseGen(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::ROSE)),
            Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::EATEN_ROSE)), TextureType::ROSE,
            ground_, 0.0f, 0.0f),

   cardinal_bird_sound_(SoundEngine::SoundEffect::CARDINAL_BIRD, 10000),
   canary_bird_sound_(SoundEngine::SoundEffect::CANARY0, 4000),
   canary2_bird_sound_(SoundEngine::SoundEffect::CANARY1, 7000),
   woodpecker_bird_sound_(SoundEngine::SoundEffect::WOODPECKER0, 3000),
   butterflyGen(attribute_location_map_, mesh_loader_),
   firefly_system_(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::FIREFLY)), 
         Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::FIREFLY)),
         TextureType::FIREFLY,
         glm::vec3(-20.0f, 0.f, -20.0f), 15),

   rain_system_(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::RAIN)),
            glm::vec3(0.0f, 100.0f, 0.0f), 2000),

   lightning_trigger_(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::LIGHTNING)),
            glm::vec3(52.0f, 10.0f, 50.0f), 0.5),

   god_rays_(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::GOD_RAYS)),
            glm::vec3(0.0f, 0.0f, 0.0f), 1.5),

   skybox(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::SKYBOX))),

   deerCam(Camera(glm::vec3(300.0f), glm::vec3(0.0f))),

   deferred_diffuse_fbo_(kScreenWidth, kScreenHeight, DEFERRED_DIFFUSE_TEXTURE, FBOType::COLOR_WITH_DEPTH),
   deferred_position_fbo_(kScreenWidth, kScreenHeight, DEFERRED_POSITION_TEXTURE, FBOType::COLOR_WITH_DEPTH),
   deferred_normal_fbo_(kScreenWidth, kScreenHeight, DEFERRED_NORMAL_TEXTURE, FBOType::COLOR_WITH_DEPTH),
   shadow_map_fbo_(kScreenWidth, kScreenHeight, SHADOW_MAP_TEXTURE, FBOType::COLOR_WITH_DEPTH),

   water_(Mesh::fromAssimpMesh(attribute_location_map_, mesh_loader_.loadMesh(MeshType::WATER))),
   song_path_(sound_engine_.loadSong(SoundEngine::Song::DAY_SONG),
         Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::GEM))),

   screen_plane_mesh_(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::PLANE))),
   title_texture_(TextureType::TITLE, DIFFUSE_TEXTURE),

   pinecone_(Mesh::fromAssimpMesh(attribute_location_map_,
            mesh_loader_.loadMesh(MeshType::PINECONE)),
         ground_,
         glm::vec2(40)),
   redRatio(1.0f),
   blueRatio(1.0f),
   greenRatio(1.0f)
{
   BoundingRectangle::loadBoundingMesh(mesh_loader_, attribute_location_map_);
   std::vector<GameObject*> objects;
   for (auto& tree : treeGen.getTrees()) {
      objects.push_back(&tree);
   }
   for (auto& bush : bushGen.getBushes()) {
      objects.push_back(&bush);
   }
   for (auto& rock : rockGen.getRocks()) {
      objects.push_back(&rock);
   }
   for (auto& flower : daisyGen.getFlowers()) {
      objects.push_back(&flower);
   }
   for (auto& flower : roseGen.getFlowers()) {
      objects.push_back(&flower);
   }
   objTree.calculateTree(objects);
}

void Game::step(units::MS dt) {
   if (day_cycle_.isDaytime()) {
      cardinal_bird_sound_.step(dt, sound_engine_);
      canary_bird_sound_.step(dt, sound_engine_);
      canary2_bird_sound_.step(dt, sound_engine_);
      woodpecker_bird_sound_.step(dt, sound_engine_);
   }

   sound_engine_.set_listener_position(deer_.getPosition(), deer_.getFacing());

   if (far_plane_target) {
      gFar = far_plane_spring.step(dt, *far_plane_target, gFar);
   }

   if (current_mode == PLAY) {
      butterflies.remove_if(ButterflySystem::checkIfEmpty);
      for (auto& butSys : butterflies) {
         butSys.step(dt);
      }

      firefly_system_.step(dt);

      rain_system_.step(dt);

      if(numLightning) {
         countLightning += dt/100.0;
         if(countLightning < 0.0)
            lighting = 0;
         else if(countLightning >= 0.0 && countLightning <= 1.0)
            lighting = 1;
         else if(countLightning > 1.0) {
            countLightning = -2.0;
            numLightning--;
            if(numLightning == 1)
               countLightning = -7.5;
         }
      } else {
         lighting = 0;
      }

      bool deerBlocked = false;
      if (deer_.isMoving()) {
         BoundingRectangle nextDeerRect = deer_.getNextBoundingBox(dt);
         std::vector<GameObject*> collObjs = objTree.getCollidingObjects(nextDeerRect);
         for (auto collObj : collObjs) {
            collObj->performObjectHit(sound_engine_);
            deerBlocked = deerBlocked || collObj->isBlocker();
         }
      }

      if(ground_.heightAt(deer_.getPosition()) < 0.0f) { //enter water
         if(!deerInWater) {
            deerInWater = true;
            sound_engine_.playSoundEffect(
            SoundEngine::SoundEffect::WATER,
            false,
            deer_.getPosition());
         }
      }
      else if(deerInWater) { //leave water
         deerInWater = false;
         sound_engine_.playSoundEffect(
            SoundEngine::SoundEffect::WATER,
            false,
            deer_.getPosition());
      }

      if (deerBlocked) {
         deer_.block();
      } else {
         deer_.step(dt, ground_, sound_engine_);
      }
      pinecone_.step(dt, ground_);
      for (auto& tree : treeGen.getTrees()) {
         if (pinecone_.bounding_rectangle().collidesWith(tree.getBoundingRectangle())) {
            pinecone_.reverse();
         }
      }

      song_path_.step(dt, deer_.bounding_rectangle());

      for (auto& bush : bushGen.getBushes()) {
         if(bush.step(dt) && butterflies.size() < MAX_NUM_BUTTERFLY_SYSTEMS && day_cycle_.isDaytime()) {
            butterflies.push_back(butterflyGen.getSystem(bush.getPosition()));
         }
      }

      for (auto& tree : treeGen.getTrees()) {
         tree.step(dt);
      }

      if(deer_.front_feet_bounding_rectangle().collidesWith(pinecone_.bounding_rectangle())) {
         deer_.stop_pounce();
         pinecone_.kick(glm::vec2(deer_.getFacing().x, deer_.getFacing().z));
      }

      for(auto& flower : daisyGen.getFlowers()) {
         if(eatFlower && deer_.head_bounding_rectangle().collidesWith(flower.bounding_rectangle())) {
            deer_.eat(flower);
         }
      }
      for(auto& flower : roseGen.getFlowers()) {
         if(eatFlower && deer_.head_bounding_rectangle().collidesWith(flower.bounding_rectangle())) {
            deer_.eat(flower);
         }
      }
      eatFlower = false;

      if (!pinecone_.been_pounced() && deer_.bounding_rectangle().collidesWith(pinecone_.aoe_bounding_rectangle())) {
         deer_.pounce(pinecone_.bounding_rectangle().getCenter());
         pinecone_.deer_pounces();
      }

      if(deer_.bounding_rectangle().collidesWith(lightning_trigger_.bounding_rectangle())) {
         if (numLightning == 0) {
            if (!raining) {
               song_path_.set_song(sound_engine_.loadSong(SoundEngine::Song::STORM_SONG));
               song_path_.reset();
            }
            raining = 1;
            lighting = 1;
            numLightning = 3;
            sound_engine_.playSoundEffect(SoundEngine::SoundEffect::THUNDER_STRIKE, false, glm::vec3());
         }
      }

      if (deer_.bounding_rectangle().collidesWith(day_night_boxes_.bounding_rectangle_moon())) {
         if (day_cycle_.isDaytime()) {
            song_path_.set_song(sound_engine_.loadSong(SoundEngine::Song::NIGHT_SONG));
            song_path_.reset();
         }
         day_cycle_.dayToNight();
      }
      else if (deer_.bounding_rectangle().collidesWith(day_night_boxes_.bounding_rectangle_sun())) {
         if (!day_cycle_.isDaytime()) {
            song_path_.set_song(sound_engine_.loadSong(SoundEngine::Song::DAY_SONG));
            song_path_.reset();
         }
         day_cycle_.nightToDay();
      }
      day_cycle_.autoAdjustTime(dt);

      Camera::Position cam_pos;
      glm::vec3 target_pos(deer_.getPosition());
      if (deer_.is_sleeping()) {
         cam_pos = Camera::Position::FRONT_RIGHT;
      } else if (deer_.is_eating()) {
         cam_pos = Camera::Position::LEFT;
      } else if (deer_.is_pouncing()) {
         cam_pos = Camera::Position::RIGHT;
      } else {
         cam_pos = Camera::Position::BEHIND;
      }
      if (deer_.is_sleeping() || deer_.is_eating()) {
         glm::vec2 head_pos(deer_.head_bounding_rectangle().getCenter());
         target_pos.x = head_pos.x;
         target_pos.z = head_pos.y;
      } else if (deer_.is_pouncing()) {
         glm::vec2 target_xz((pinecone_.bounding_rectangle().getCenter() + deer_.bounding_rectangle().getCenter()) / 2.f);
         target_pos.x = target_xz.x;
         target_pos.z = target_xz.y;
      }

      if(isFadingFlowers || redRatio < 1.0 || blueRatio < 1.0) {
         float stepScale = 1.0;

         if(!isFadingFlowers || flowerResetting) //Pushed 'G' to stop greyscale. Gradual return to color.
            stepScale = -15.0;
         else if(deerInWater)
            stepScale = -10.0;
         else if(redFlowerTimer < fadingTimeBeforeWater)
            stepScale = 8.0;
         else
            fadingTimeBeforeWater = -1.0;

         if(deerInWater && fadingTimeBeforeWater < 0.0) {
            fadingTimeBeforeWater = redFlowerTimer;
         }

         redFlowerTimer = std::min(MAX_FLOWER_TIME, redFlowerTimer + stepScale * (dt * COLOR_LOSS_RATE));
         if (redFlowerTimer < 0.0) {
            redFlowerTimer = 0.0;
            flowerResetting = false;
         }


         redRatio = std::max(COLOR_RATIO_MIN, (MAX_FLOWER_TIME - redFlowerTimer) / MAX_FLOWER_TIME);
         blueRatio = std::max(COLOR_RATIO_MIN, (MAX_FLOWER_TIME - redFlowerTimer) / MAX_FLOWER_TIME);
         greenRatio = std::max(COLOR_RATIO_MIN, ((MAX_FLOWER_TIME - redFlowerTimer) / MAX_FLOWER_TIME + 
                  (MAX_FLOWER_TIME - redFlowerTimer) / MAX_FLOWER_TIME) / 2);

      }

      deerCam.step(dt, target_pos, deer_.getFacing(), cam_pos);
   } else if (current_mode == START) {
      deerCam.circle(dt, deer_.getPosition());
   }
}

void Game::draw() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   const auto sunIntensity = day_cycle_.getSunIntensity();

   float checkNight = 0.5f;
   if(day_cycle_.isDay())
      checkNight = 1.0;
   glClearColor (0.05098 * checkNight,
         0.6274509 * checkNight,
         1.0 * checkNight, 1.0f);

   std::vector<Drawable> drawables;
   if (draw_collision_box) {
      Drawable br_drawable;
      br_drawable.draw_template = BoundingRectangle::draw_template();
      br_drawable.draw_instances.push_back(deer_.bounding_rectangle().model_matrix());
      br_drawable.draw_instances.push_back(lightning_trigger_.bounding_rectangle().model_matrix());
      br_drawable.draw_instances.push_back(day_night_boxes_.bounding_rectangle_sun().model_matrix());
      br_drawable.draw_instances.push_back(day_night_boxes_.bounding_rectangle_moon().model_matrix());
      for (auto& bush : bushGen.getBushes()) {
         br_drawable.draw_instances.push_back(bush.getBoundingRectangle().model_matrix());
      }
      for (auto& bush : bushGen.getBushes()) {
         br_drawable.draw_instances.push_back(bush.getBoundingRectangle().model_matrix());
      }
      for (auto& tree : treeGen.getTrees()) {
         br_drawable.draw_instances.push_back(tree.getBoundingRectangle().model_matrix());
      }
      for (auto& flower : daisyGen.getFlowers()) {
         br_drawable.draw_instances.push_back(flower.getBoundingRectangle().model_matrix());
      }
      for (auto& flower : roseGen.getFlowers()) {
         br_drawable.draw_instances.push_back(flower.getBoundingRectangle().model_matrix());
      }
      for (auto& rock : rockGen.getRocks()) {
         br_drawable.draw_instances.push_back(rock.getBoundingRectangle().model_matrix());
      }
      for (auto& br : song_path_.bounding_rectangles()) {
         br_drawable.draw_instances.push_back(br.model_matrix());
      }
      br_drawable.draw_instances.push_back(pinecone_.bounding_rectangle().model_matrix());
      br_drawable.draw_instances.push_back(pinecone_.aoe_bounding_rectangle().model_matrix());
      drawables.push_back(br_drawable);
   }


   Drawable screen_drawable;
   BoundingRectangle screen_br = BoundingRectangle(glm::vec2(0.0f, 0.0f),
            glm::vec2(kScreenWidthf, kScreenHeightf), 0.0f);
   screen_drawable.draw_template = BoundingRectangle::draw_template();
   screen_drawable.draw_template.mesh = screen_plane_mesh_;
   screen_drawable.draw_instances.push_back(screen_br.model_matrix_screen());

   /* Final Pass Plane */
   screen_drawable.draw_template.texture = deer_.drawable().draw_template.texture;
   screen_drawable.draw_template.shader_type = ShaderType::FINAL_LIGHT_PASS;
   drawables.push_back(screen_drawable);

   if(displayTitleScreen) {
      /* Title Screen Plane */
      for(auto& instance : screen_drawable.draw_instances) {
         instance.model_transform = glm::translate(glm::mat4(), glm::vec3(-1.0, 0.0, 0.0)) * instance.model_transform;
      }
      screen_drawable.draw_template.texture = title_texture_;
      screen_drawable.draw_template.effects = EffectSet({EffectType::IS_TITLE_SCREEN});
      screen_drawable.draw_template.shader_type = ShaderType::SKYBOX;
      drawables.push_back(screen_drawable);
   }


   drawables.push_back(deer_.drawable());
   {
      const auto d = deer_.dust_drawable();
      drawables.insert(drawables.end(), d.begin(), d.end());
   }

   drawables.push_back(ground_.drawable());
   drawables.push_back(treeGen.drawable());

   if (current_mode == PLAY) {
      drawables.push_back(pinecone_.drawable());
      drawables.push_back(lightning_trigger_.drawable());
      drawables.push_back(day_night_boxes_.drawableSun());
      drawables.push_back(day_night_boxes_.drawableMoon());

      drawables.push_back(bushGen.drawable());
      drawables.push_back(rockGen.drawable());

      drawables.push_back(daisyGen.drawable());
      drawables.push_back(daisyGen.drawableEaten());
      drawables.push_back(roseGen.drawable());
      drawables.push_back(roseGen.drawableEaten());

      drawables.push_back(song_path_.drawable());

      if (gReflections) {
         Drawable waterDrawable = water_.drawable();
         if(useTextureShader)
            waterDrawable.draw_template.shader_type = ShaderType::TEXTURE;
         drawables.push_back(waterDrawable);
      }

      for (auto& butSys : butterflies) {
         drawables.push_back(butSys.drawable());
      }

      god_rays_.setRayPositions(song_path_.CurrentStonePosition(), song_path_.NextStonePosition());
      god_rays_.setCurrentRayScale(song_path_.CurrentStoneRemainingRatio());
      drawables.push_back(god_rays_.drawable());

      if(!day_cycle_.isDay()) {
         drawables.push_back(firefly_system_.drawable());
         drawables.push_back(firefly_system_.drawable_glow());
      }

      drawables.push_back(treeGen.leafDrawable());
   }

   // View Frustum Culling
   auto viewMatrix = deerCam.getViewMatrix();
   const auto view_projection = gProjectionMatrix * viewMatrix;
   Frustum frustum(view_projection);
   auto culledDrawables = frustum.cullDrawables(drawables);

   // Skybox is never culled, so we add it after.
   culledDrawables.push_back(CulledDrawable::fromDrawable(skybox.drawable(day_cycle_.isDay(), deerCam.get_position())));

   glBindFramebuffer(GL_FRAMEBUFFER, 0);

   if(useTextureShader) {
      for(auto& drawables : culledDrawables) {
         if(drawables.draw_template.shader_type == ShaderType::DEFERRED)
            drawables.draw_template.shader_type = ShaderType::TEXTURE;
      }
   }

   const auto deerPos = deer_.getPosition();
   const auto sunDir = day_cycle_.getSunDir();
   float ratioMax = redRatio > blueRatio ? redRatio : blueRatio;
   if(ratioMax > 1.0)
      ratioMax = 1.0;

   glm::vec3 flowerFade = glm::vec3(ratioMax);

   draw_shader_.Draw(flowerFade,
                     shadow_map_fbo_,
                     water_.fbo(),
                     deferred_diffuse_fbo_,
                     deferred_position_fbo_,
                     deferred_normal_fbo_,
                     culledDrawables, viewMatrix, switchBlinnPhongShading,
                     deerPos, sunDir, sunIntensity, lighting);
}

void Game::mainLoop() {
   Input input;
   showTreeShadows = false;
   bool running = true;
   SDL_Event event;
   units::MS previous_time = SDL_GetTicks();

   while (running) {
      {  // Collect input
         input.beginFrame();
         while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
               running = false;
            }

            if (event.type == SDL_KEYDOWN && event.key.repeat == false) {
               input.keyDown(event.key);
            } else if (event.type == SDL_KEYUP) {
               input.keyUp(event.key);
            }
         }
      }
      { // Handle input
         if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE)) {
            running = false;
         }
         { // show or hide tree shadows -- Katelyn
            const auto key_tree = SDL_SCANCODE_T;
            if (input.wasKeyPressed(key_tree)) {
               showTreeShadows = !showTreeShadows;
               treeGen.includeInShadows(showTreeShadows);
            }
         }
         { // handle walk forward/backward for deer.
            const auto key_forward = SDL_SCANCODE_W;
            const auto key_backward = SDL_SCANCODE_S;
            if (input.isKeyHeld(key_forward) && !input.isKeyHeld(key_backward)) {
               deer_.walkForward();
            } else if (input.isKeyHeld(key_backward) && !input.isKeyHeld(key_forward)) {
               deer_.walkBackward();
            } else {
               deer_.stopWalking();
            }
         }
         { // handle strafe left/right for deer.
            const auto key_left = SDL_SCANCODE_A;
            const auto key_right = SDL_SCANCODE_D;
            if (input.isKeyHeld(key_left) && !input.isKeyHeld(key_right)) {
               deer_.turnLeft();
            } else if (!input.isKeyHeld(key_left) && input.isKeyHeld(key_right)) {
               deer_.turnRight();
            } else {
               deer_.stopTurning();
            }
         }
         { // handle jumping
            const auto key_jump = SDL_SCANCODE_F;
            if (input.wasKeyPressed(key_jump)) {
               deer_.jump();
            }
         }
         { // handle jumping
            const auto key_eat = SDL_SCANCODE_E;
            if (input.wasKeyPressed(key_eat)) {
               eatFlower = true;
            }
         }
         { // handle jumping
            const auto key_title = SDL_SCANCODE_T;
            if (input.wasKeyPressed(key_title)) {
               displayTitleScreen = !displayTitleScreen;
            }
         }
         {
            const auto key_toNight = SDL_SCANCODE_3;
            if (input.wasKeyPressed(key_toNight)) {
               day_cycle_.dayToNight();
            }
         }
         {
            const auto key_toDay = SDL_SCANCODE_4;
            if (input.wasKeyPressed(key_toDay)) {
               day_cycle_.nightToDay();
            }
         }
         { // Lightning
            const auto key_lightning = SDL_SCANCODE_L;
            if (input.wasKeyPressed(key_lightning)) {
               raining = 1;
               lighting = 1;
               numLightning = 3;
               sound_engine_.playSoundEffect(SoundEngine::SoundEffect::THUNDER_STRIKE, false, glm::vec3());
            }
         }
         { // Rain
            const auto key_rain = SDL_SCANCODE_R;
            if (input.wasKeyPressed(key_rain)) {
               raining = !raining;
            }
         }
         { // on/off fading saturation for flowers
            const auto key_fade = SDL_SCANCODE_G;
            if (input.wasKeyPressed(key_fade)) {
               isFadingFlowers = !isFadingFlowers;
            }
         }
         { //Change shading models
            const auto key_blinn = SDL_SCANCODE_B;
            if (input.wasKeyPressed(key_blinn)) {
               switchBlinnPhongShading = !switchBlinnPhongShading;
            }
         }
         { //handle quit
            const auto key_quit = SDL_SCANCODE_Q;
            if (input.wasKeyPressed(key_quit)) {
               running = false;
            }
         }
      }
      if (current_mode == START) {
         { // start button
            const auto key_start = SDL_SCANCODE_RETURN;
            if (input.wasKeyPressed(key_start)) {
               far_plane_target = 400.f;
               current_mode = PLAY;
               if (!gReflections) {
                  gReflections = true;
               }
               displayTitleScreen = !displayTitleScreen;
               useTextureShader = false;
            }
         }
      }
      else if (current_mode == PLAY) {
         { // handle walk forward/backward for deer.
            const auto key_forward = SDL_SCANCODE_W;
            const auto key_backward = SDL_SCANCODE_S;
            if (input.isKeyHeld(key_forward) && !input.isKeyHeld(key_backward)) {
               deer_.walkForward();
            } else if (input.isKeyHeld(key_backward) && !input.isKeyHeld(key_forward)) {
               deer_.walkBackward();
            } else {
               deer_.stopWalking();
            }
         }
         { // handle strafe left/right for deer.
            const auto key_left = SDL_SCANCODE_A;
            const auto key_right = SDL_SCANCODE_D;
            if (input.isKeyHeld(key_left) && !input.isKeyHeld(key_right)) {
               deer_.turnLeft();
            } else if (!input.isKeyHeld(key_left) && input.isKeyHeld(key_right)) {
               deer_.turnRight();
            } else {
               deer_.stopTurning();
            }
         }
         {
            const auto key_jump = SDL_SCANCODE_F;
            if (input.wasKeyPressed(key_jump)) {
               deer_.jump();
            }
         }
         {
            const auto key_eat = SDL_SCANCODE_E;
            if (input.wasKeyPressed(key_eat)) {
               eatFlower = true;
            }
         }
      }

      {
         const units::MS current_time = SDL_GetTicks();
         const units::MS dt = current_time - previous_time;
         { // Debug/perf stuff.
            if (input.wasKeyPressed(SDL_SCANCODE_F12)) {
               useTextureShader = !useTextureShader;
            }
            if (input.wasKeyPressed(SDL_SCANCODE_F11)) {
               show_fps = !show_fps;
            }
            if (input.wasKeyPressed(SDL_SCANCODE_F10)) {
               draw_collision_box = !draw_collision_box;
            }
            if (input.wasKeyPressed(SDL_SCANCODE_F9)) {
               gReflections = !gReflections;
            }

            if (input.wasKeyPressed(SDL_SCANCODE_F1)) {
               adjust_mode = AdjustType::FAR;
               std::clog << "adjusting far-plane" << std::endl;
            }
            if (input.wasKeyPressed(SDL_SCANCODE_F2)) {
               adjust_mode = AdjustType::FOV;
               std::clog << "adjusting field-of-view" << std::endl;
            }
            if (input.wasKeyPressed(SDL_SCANCODE_F3)) {
               adjust_mode = AdjustType::NEAR;
               std::clog << "adjusting near-plane" << std::endl;
            }
            if (input.wasKeyPressed(SDL_SCANCODE_F4)) {
               adjust_mode = AdjustType::CAM_HEIGHT;
               std::clog << "adjusting camera's height above deer" << std::endl;
            }
            if (input.wasKeyPressed(SDL_SCANCODE_F5)) {
               adjust_mode = AdjustType::CAM_DIST;
               std::clog << "adjusting camera's distance to deer" << std::endl;
            }
            if (input.wasKeyPressed(SDL_SCANCODE_F6)) {
               adjust_mode = AdjustType::HEIGHT_MAP;
               std::clog << "adjusting height map's scale" << std::endl;
            }
            if (input.wasKeyPressed(SDL_SCANCODE_U)) {
               gDensityLevel = std::max(gDensityLevel - 1, 0);
            }
            if (input.wasKeyPressed(SDL_SCANCODE_I)) {
               gDensityLevel = std::min(gDensityLevel + 1, kMaxDensityLevel);
               std::clog << "adjusting height map's scale" << std::endl;
            }
            {
               const auto far_adjust_speed = 100.f / 1000.f;
               const auto near_adjust_speed = 10.f / 1000.f;
               const auto fov_adjust_speed = 15.f / 1000.f;
               const auto camera_dist_speed = 5.f / 1000.f;
               const auto height_map_speed = 5.f / 1000.f;
               if (input.isKeyHeld(SDL_SCANCODE_K)) { // adjust up
                  switch (adjust_mode) {
                     case AdjustType::FAR:
                        gFar += far_adjust_speed * dt;
                        far_plane_target = boost::none;
                        break;
                     case AdjustType::NEAR:
                        gNear += near_adjust_speed * dt;
                        break;
                     case AdjustType::FOV:
                        gFieldOfView += fov_adjust_speed * dt;
                        break;
                     case AdjustType::CAM_HEIGHT:
                        cameraHeightAboveDeer += camera_dist_speed * dt;
                        break;
                     case AdjustType::CAM_DIST:
                        cameraDistanceToDeer += camera_dist_speed * dt;
                        break;
                     case AdjustType::HEIGHT_MAP:
                        gHeightMapScale += height_map_speed * dt;
                        break;
                  }
               }
               if (input.isKeyHeld(SDL_SCANCODE_J)) { // adjust down
                  switch (adjust_mode) {
                     case AdjustType::FAR:
                        gFar -= far_adjust_speed * dt;
                        far_plane_target = boost::none;
                        break;
                     case AdjustType::NEAR:
                        gNear -= near_adjust_speed * dt;
                        break;
                     case AdjustType::FOV:
                        gFieldOfView -= fov_adjust_speed * dt;
                        break;
                     case AdjustType::CAM_HEIGHT:
                        cameraHeightAboveDeer -= camera_dist_speed * dt;
                        break;
                     case AdjustType::CAM_DIST:
                        cameraDistanceToDeer -= camera_dist_speed * dt;
                        break;
                     case AdjustType::HEIGHT_MAP:
                        gHeightMapScale -= height_map_speed * dt;
                        break;
                  }
               }
               gProjectionMatrix = calculateProjection();
            }
         }
         step(dt);
         previous_time = current_time;
      }

      {
         if (show_fps)
            draw_timer.start();
         draw();
         if (show_fps)
            draw_timer.end();
         engine_.swapWindow();
      }

      SDL_Delay(5);
   }
}
