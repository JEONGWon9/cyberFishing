#include "iImage.h"

#include "iStd.h"

iImage::iImage()
{
	arrayTex = new iArray(freeTex);
	tex = NULL;
	position = iPointMake(0, 0);

	selected = false;
	selectedDt = 0.0f;
	_selectedDt = default_selectedDt;
	selectedScale = default_selectedScale;

	animation = false;
	aniDt = 0.0f;
	_aniDt = 0.08f;
	frame = 0;
	repeatNum = 0;
	_repeatNum = 0;// inf
	method = NULL;

	anc = TOP | LEFT;
	leftRight = 0;
	degree = 0.0f;
	scale = 1.0f;
}

iImage::~iImage()
{
	delete arrayTex;
}

void iImage::freeTex(void* data)
{
	Texture* tex = (Texture*)data;
	if (tex->retainCount > 1)
	{
		tex->retainCount--;
		return;
	}
	freeImage(tex);
}

void iImage::addObject(Texture* tex)
{
	arrayTex->addObject(tex);
	tex->retainCount++;
	if (this->tex == NULL)
		this->tex = tex;
}

void iImage::setTexAtIndex(int index)
{
	Texture* tex = (Texture*)arrayTex->objectAtIndex(index);
	if (tex)
	{
		this->tex = tex;
		frame = index;
	}
}

void iImage::replaceAtIndex(int index, Texture* tex)
{
	Texture* t = (Texture*)arrayTex->objectAtIndex(index);
#if 0
	arrayTex->remove(index);
	arrayTex->addObject(index, tex);
	if (this->tex == t)
		this->tex = tex;
#else
	//freeImage(t);
	if (t->retainCount > 1)
		t->retainCount--;
	else
		glDeleteTextures(1, &t->texID);
	memcpy(t, tex, sizeof(Texture));
	free(tex);
#endif
}

void iImage::paint(float dt, iPoint off)
{
	if (animation)
	{
		aniDt += dt;
		if (aniDt < _aniDt)
		{
			;
		}
		else
		{
			aniDt -= _aniDt;
			frame++;
			if (frame == arrayTex->count)
			{
				if( _repeatNum==0 )
					frame = 0;
				else
				{
					repeatNum++;
					if (repeatNum < _repeatNum)
						frame = 0;
					else// if (repeatNum == _repeatNum)
					{
						if (method)
							method(this);
						if (lastFrame)
							frame = arrayTex->count - 1;
						else
							frame = 0;
						animation = false;
					}
				}
			}
			tex = (Texture*)arrayTex->objectAtIndex(frame);
		}
	}

	if (selected)
	{
		selectedDt += dt;
		if (selectedDt > _selectedDt)
			selectedDt = _selectedDt;
	}
	else
	{
		selectedDt -= dt;
		if (selectedDt < 0.0f)
			selectedDt = 0.0f;
	}
	iPoint p = position + off;
	float s = 1.0f + linear(selectedDt / _selectedDt, 0.0f, selectedScale);
	s *= scale;

	int reverse;
	switch (leftRight) {
	case 0: reverse = REVERSE_NONE; break;
	case 1:	reverse = REVERSE_WIDTH; break;
	case 2:	reverse = REVERSE_HEIGHT; break;
	}

	int x = p.x;
	int y = p.y;
	int width = tex->width;
	int height = tex->height;
	switch (anc) {
	case TOP | LEFT:                                         break;
	case TOP | HCENTER:     x -= width / 2;                  break;
	case TOP | RIGHT:       x -= width;                      break;
	case VCENTER | LEFT:                    y -= height / 2; break;
	case VCENTER | HCENTER: x -= width / 2; y -= height / 2; break;
	case VCENTER | RIGHT:   x -= width;     y -= height / 2; break;
	case BOTTOM | LEFT:                     y -= height;     break;
	case BOTTOM | HCENTER:  x -= width / 2; y -= height;     break;
	case BOTTOM | RIGHT:    x -= width;     y -= height;     break;
	}

	if (s == 0.0f)
	{
		//drawImage(tex, x, y, TOP | LEFT);
		drawImage(tex, x, y, 0, 0, tex->width, tex->height,
			TOP | LEFT, 1, 1, 2, degree, reverse);
	}
	else
	{
		p.x += tex->width / 2;
		p.y += tex->height / 2;
		drawImage(tex, x, y, 0, 0, tex->width, tex->height,
			VCENTER | HCENTER, s, s, 2, degree, reverse);
	}
}

void iImage::startAnimation(IMAGE_METHOD m)
{
	animation = true;
	method = m;
	aniDt = 0.0f;
	frame = 0;
	repeatNum = 0;
}

iRect iImage::touchRect(iPoint p)
{
	p += position;
	iRect rt = iRectMake(p.x, p.y,
						tex->width, tex->height);
	return rt;
}

iImage* iImage::copy()
{
#if 0
	iArray* array = new iArray(freeTex);
	for (int i = 0; i < arrayTex->count; i++)
	{
		//array->addObject(arrayTex->objectAtIndex(i));
		Texture* tex = (Texture*)arrayTex->objectAtIndex(i);
		array->addObject(tex);
		tex->retainCount++;
	}

	iImage* img = new iImage();
	memcpy(img, this, sizeof(iImage));
	img->arrayTex = array;
#else
	iImage* img = new iImage();
	memcpy(img, this, sizeof(iImage));
	img->arrayTex = new iArray(freeTex);
	for (int i = 0; i < arrayTex->count; i++)
		img->addObject((Texture*)arrayTex->objectAtIndex(i));
#endif

	return img;
}

