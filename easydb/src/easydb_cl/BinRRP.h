#ifndef _EASYDB_BIN_RRP_H_
#define _EASYDB_BIN_RRP_H_

#include "ICommand.h"

namespace edb
{

class BinRRP : public ICommand
{
public:
	BinRRP() {}

	//
	// interface ITask
	//
	virtual std::string Command() const;
	virtual std::string Description() const;
	virtual std::string Usage() const;
	virtual void Run(int argc, char *argv[]);

	static ICommand* Create() { return new BinRRP(); }

private:
	void Trigger(const std::string& json_file, const std::string& image_id_file);

}; // PackRRPBin

}

#endif // _EASYDB_BIN_RRP_H_