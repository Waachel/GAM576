//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------

#ifndef FONT_SPRITE_H
#define FONT_SPRITE_H

#include "GameObjectRigid.h"

#include "Sprite.h"
#include "Rect.h"
#include "Glyph.h"
#include "Color.h"

namespace Azul
{
	class FontSprite : public Sprite
	{
	public:
		enum class Name
		{
			TestMessage,
			TestOneOff,

			NullObject,
			Uninitialized
		};

	public:

		FontSprite(GraphicsObject *graphicsObject);

		FontSprite() = delete;
		FontSprite(const FontSprite &) = delete;
		FontSprite &operator = (const FontSprite &) = delete;
		virtual ~FontSprite() override;

		void Set(FontSprite::Name name, 
				 const char *pMessage, 
				 Glyph::Name glyphName, 
				 float xStart, 
				 float yStart);

		void Set(FontSprite::Name name,
				 const char *pMessage,
				 Glyph::Name glyphName,
				 float xStart,
				 float yStart,
				 Color &color);

		void ChangeMessage(const char* _pMessage, float xStart, float yStart);

		virtual void Draw() override;
		virtual void Update(AnimTime currTime) override;

	private:
		void privUpdate(AnimTime currTime);

	public:
		// data: -----------------------------------------------------------------------

		FontSprite::Name name;
		Rect *pScreenRect;
		char *poMessage;
		Glyph::Name glyphName;

		float x;
		float y;

	};

}

#endif

//--- End of File ---

