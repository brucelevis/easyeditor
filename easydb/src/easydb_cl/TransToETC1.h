#ifndef _EASYDB_TRANS_TO_ETC1_H_
#define _EASYDB_TRANS_TO_ETC1_H_

#include "ICommand.h"

#include <wx/string.h>

namespace edb
{

class TransToETC1 : public ICommand
{
public:
	TransToETC1() {}

	//
	// interface ICommand
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual int Run(int argc, char *argv[]);

	static ICommand* Create() { return new TransToETC1(); }

private:
	void Trigger(const std::string& path);

	void Format(const std::string& filepath);

}; // TransToETC1

}

#endif // _EASYDB_TRANS_TO_ETC1_H_