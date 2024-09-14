//----------------------------------------------------------------------------
// Copyright 2023, Ed Keenan, all rights reserved.
//----------------------------------------------------------------------------
#include "FontData.h"

namespace Azul
{
	FontData::~FontData()
	{
		delete[] this->poFontEntry;
		this->poFontEntry = nullptr;
	}

	FontData::FontData()
	{
		this->numKeys = 0;
		this->poFontEntry = nullptr;
	}

	FontData::FontData(unsigned int _numKeys)
	{
		this->numKeys = _numKeys;

		this->poFontEntry = new FontEntry[_numKeys]();
		assert(this->poFontEntry);
	}

	FontData &FontData::operator = (const FontData &r)
	{
		if(this != &r)
		{
			// Nuke orginal
			this->numKeys = 0;
			delete[] this->poFontEntry;

			// copy the new
			this->numKeys = r.numKeys;
			this->poFontEntry = new FontEntry[r.numKeys]();

			for(size_t i = 0; i < r.numKeys; i++)
			{
				this->poFontEntry[i] = r.poFontEntry[i];
			}
		}

		return *this;
	}

	void FontData::Serialize(FontData_proto &out) const
	{
		AZUL_UNUSED_VAR(out);

		// Add the number of bones
		out.set_numkeys(this->numKeys);

		// Add the SkelEntry
		FontEntry_proto tmpBoneEntry_proto;

		for(unsigned int i = 0; i < this->numKeys; i++)
		{
			// create one proto
			this->poFontEntry[i].Serialize(tmpBoneEntry_proto);

			// add it to the message(package)
			*out.add_pofontentry() = tmpBoneEntry_proto;
		}

		assert(out.numkeys() == this->numKeys);
	}

	void FontData::Deserialize(const FontData_proto &in)
	{
		AZUL_UNUSED_VAR(in);
		// Safety
		delete[] this->poFontEntry;
		this->poFontEntry = nullptr;

		this->numKeys = in.numkeys();

		this->poFontEntry = new FontEntry[this->numKeys]();
		assert(this->poFontEntry);

		for(unsigned int i = 0; i < this->numKeys; i++)
		{
			poFontEntry[i].Deserialize(in.pofontentry((int)i));
		}
	}

	void FontData::Print(const char *const pName) const
	{
		AZUL_UNUSED_VAR(pName);
		Trace::out("%s: \n", pName);

		Trace::out("numKeys: %d\n", this->numKeys);

		for(size_t i = 0; i < this->numKeys; i++)
		{
			this->poFontEntry[i].Print(pName);
		}

		Trace::out("\n");
	}
}

// --- End of File ---


