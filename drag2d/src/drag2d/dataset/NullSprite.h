#pragma once

#include "ISprite.h"

namespace d2d
{
	class NullSprite : public ISprite
	{
	public:
		NullSprite(ISymbol* symbol) 
			: m_symbol(symbol) {
				m_symbol->Retain();
		}
		~NullSprite() {
			if (m_symbol) {
				m_symbol->Release();
			}
		}

		//
		// IObject interface
		//
		virtual NullSprite* clone() const { return NULL; }

		//
		// ISprite interface
		//
		virtual const ISymbol& getSymbol() const { return *m_symbol; }
		virtual void setSymbol(ISymbol* symbol) { m_symbol = symbol; }
		virtual void loadBodyFromFile() {}

	private:
		ISymbol* m_symbol;

	}; // NullSprite
}