#include "shadow_map.h"
#include <stdio.h>

ShadowMapFBO::ShadowMapFBO() {
	glGenFramebuffers(1, &fbo_id);
}

bool ShadowMapFBO::setup(unsigned int WindowWidth, unsigned int WindowHeight) {
	printf("inside of fbo\n");
	glGenTextures(1, &shadow_map_texture);
	glBindTexture(GL_TEXTURE_2D, shadow_map_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, 
		WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER,
	 GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadow_map_texture, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

void ShadowMapFBO::BindForWriting() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo_id);
}

void ShadowMapFBO::BindForReading() {
	glActiveTexture(shadow_map_texture);
	glBindTexture(GL_TEXTURE_2D, shadow_map_texture);
}