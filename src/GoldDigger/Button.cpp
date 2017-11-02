#include "Button.h"



Button::Button()
{
	myText.id = "saiyan";
	myText.text = "HelloWorld";
	myText.color = { 0, 0, 0, 255 };

	fontID = SAIYAN_80;

	x = y = 0;
	width = height = 50;

	textPlaceholder = new SDL_Rect{ x, y, width, height };

	Renderer::Instance()->LoadFont(Font{ myText.id, PATH_FONT+"saiyan.ttf", 80 });
	Renderer::Instance()->LoadTextureText(myText.id, myText);
}

Button::~Button()
{
}

void Button::Update()
{
	Renderer::Instance()->PushImage(myText.id, *textPlaceholder);
}
