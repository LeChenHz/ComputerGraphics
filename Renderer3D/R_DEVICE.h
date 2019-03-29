#pragma once

//=====================================================================
// ��Ⱦ�豸
//=====================================================================

#include <assert.h>
#include "R_MATH.h"
#include "R_CoordinateTransform.h"

typedef unsigned int IUINT32;

typedef struct {
	transform_t transform;      // ����任��
	int width;                  // ���ڿ���
	int height;                 // ���ڸ߶�
	IUINT32 **framebuffer;      // ���ػ��棺framebuffer[y] ������ y��
	float **zbuffer;            // ��Ȼ��棺zbuffer[y] Ϊ�� y��ָ��
	IUINT32 **texture;          // ������ͬ����ÿ������
	int tex_width;              // ��������
	int tex_height;             // �����߶�
	float max_u;                // ���������ȣ�tex_width - 1
	float max_v;                // �������߶ȣ�tex_height - 1
	int render_state;           // ��Ⱦ״̬
	IUINT32 background;         // ������ɫ
	IUINT32 foreground;         // �߿���ɫ
}device_t;

void device_init(device_t *device, int width, int height, void *fb);
void device_set_texture(device_t *device, void *bits, long pitch, int w, int h);
void device_clear(device_t *device, int mode);

void device_draw_line(device_t *device, int x1, int y1, int x2, int y2, IUINT32 c);
void device_pixel(device_t *device, int x, int y, IUINT32 color);
IUINT32 device_texture_read(const device_t *device, float u, float v);


#define RENDER_STATE_WIREFRAME      1		// ��Ⱦ�߿� ��Ĭ�ϣ�
#define RENDER_STATE_TEXTURE        2		// ��Ⱦ����
#define RENDER_STATE_COLOR          4		// ��Ⱦ��ɫ

// �豸��ʼ����framebuffer��zbuffer
// fbΪ�ⲿ֡���棬�� NULL �������ⲿ֡���棨ÿ�� 4�ֽڶ��룩
void device_init(device_t *device, int width, int height, void *fb) {

	int need = sizeof(void*) * (height * 2 + 1024) + width * height * 8;
	char *ptr = (char*)malloc(need + 64);
	char *framebuf, *zbuf;
	int j;
	assert(ptr);
	
	device->framebuffer = (IUINT32**)ptr; //�洢height�е�ָ��
	device->zbuffer = (float**)(ptr + sizeof(void*) * height);
	ptr += sizeof(void*) * height * 2;
	device->texture = (IUINT32**)ptr;
	ptr += sizeof(void*) * 1024;
	framebuf = (char*)ptr;
	zbuf = (char*)ptr + width * height * 4;
	ptr += width * height * 8;

	if (fb != NULL) 
		framebuf = (char*)fb; //�������ⲿ֡����

	for (j = 0; j < height; j++) 
	{
		device->framebuffer[j] = (IUINT32*)(framebuf + width * 4 * j);
		device->zbuffer[j] = (float*)(zbuf + width * 4 * j);
	}
	device->texture[0] = (IUINT32*)ptr;
	device->texture[1] = (IUINT32*)(ptr + 16);
	memset(device->texture[0], 0, 64);
	device->tex_width = 2;
	device->tex_height = 2;
	device->max_u = 1.0f;
	device->max_v = 1.0f;
	device->width = width;
	device->height = height;
	device->background = 0xc0c0c0;
	device->foreground = 0;
	transform_init(&device->transform, width, height); //��ʼ��MVP����
	device->render_state = RENDER_STATE_WIREFRAME; //Ĭ�ϣ��߿�ģʽ
}

// ���õ�ǰ����
void device_set_texture(device_t *device, void *bits, long pitch, int w, int h) {
	char *ptr = (char*)bits;
	int j;
	assert(w <= 1024 && h <= 1024);
	for (j = 0; j < h; ptr += pitch, j++) 	// ���¼���ÿ��������ָ��
		device->texture[j] = (IUINT32*)ptr;
	device->tex_width = w;
	device->tex_height = h;
	device->max_u = (float)(w - 1);
	device->max_v = (float)(h - 1);
}

// ��� framebuffer �� zbuffer
// ��������mode��
void device_clear(device_t *device, int mode)
{
	int y, x, height = device->height;
	for (y = 0; y < device->height; y++) 
	{
		IUINT32 *dst = device->framebuffer[y];
		IUINT32 cc = (height - 1 - y) * 230 / (height - 1); //������ɫ���߶��������½��䣬230ֻ����ɫ�ȼ�
		cc = (cc << 16) | (cc << 8) | cc; //rgb
		if (mode == 0) 
			cc = device->background;
		for (x = device->width; x > 0; dst++, x--) 
			dst[0] = cc;
	}
	for (y = 0; y < device->height; y++) 
	{
		float *dst = device->zbuffer[y];
		for (x = device->width; x > 0; dst++, x--) 
			dst[0] = 0.0f;
	}
}

// �����߶�
void device_draw_line(device_t *device, int x1, int y1, int x2, int y2, IUINT32 c) {
	int x, y, rem = 0;
	if (x1 == x2 && y1 == y2) {
		device_pixel(device, x1, y1, c);
	}
	else if (x1 == x2) {
		int inc = (y1 <= y2) ? 1 : -1;
		for (y = y1; y != y2; y += inc) device_pixel(device, x1, y, c);
		device_pixel(device, x2, y2, c);
	}
	else if (y1 == y2) {
		int inc = (x1 <= x2) ? 1 : -1;
		for (x = x1; x != x2; x += inc) device_pixel(device, x, y1, c);
		device_pixel(device, x2, y2, c);
	}
	else {
		int dx = (x1 < x2) ? x2 - x1 : x1 - x2;
		int dy = (y1 < y2) ? y2 - y1 : y1 - y2;
		if (dx >= dy) {
			if (x2 < x1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
			for (x = x1, y = y1; x <= x2; x++) {
				device_pixel(device, x, y, c);
				rem += dy;
				if (rem >= dx) {
					rem -= dx;
					y += (y2 >= y1) ? 1 : -1;
					device_pixel(device, x, y, c);
				}
			}
			device_pixel(device, x2, y2, c);
		}
		else {
			if (y2 < y1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
			for (x = x1, y = y1; y <= y2; y++) {
				device_pixel(device, x, y, c);
				rem += dx;
				if (rem >= dy) {
					rem -= dy;
					x += (x2 >= x1) ? 1 : -1;
					device_pixel(device, x, y, c);
				}
			}
			device_pixel(device, x2, y2, c);
		}
	}
}

// ���㣨�ı���Ӧ��framebuffer[][]��ֵ��
void device_pixel(device_t *device, int x, int y, IUINT32 color) 
{
	if (((IUINT32)x) < (IUINT32)device->width && ((IUINT32)y) < (IUINT32)device->height) 
	{
		device->framebuffer[y][x] = color;
	}
}

// ���������ȡ����
IUINT32 device_texture_read(const device_t *device, float u, float v)
{
	int x, y;
	u = u * device->max_u;
	v = v * device->max_v;
	x = (int)(u + 0.5f);
	y = (int)(v + 0.5f);
	x = CMID(x, 0, device->tex_width - 1);
	y = CMID(y, 0, device->tex_height - 1);
	return device->texture[y][x];
}