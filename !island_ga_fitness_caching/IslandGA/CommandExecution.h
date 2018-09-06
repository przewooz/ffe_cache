#ifndef COMMAND_EXECUTION_H
#define COMMAND_EXECUTION_H

#include "CommandParamParser.h"
#include "Error.h"
#include "Log.h"

#include <atlstr.h>
#include <cstdint>
#include <iostream>

using namespace std;

class CCommandExecution
{
public:
	CCommandExecution(CString sName, istream *psSettings, CLog *pcLog, ostream *psSummary);

	virtual ~CCommandExecution();

	virtual CError eExecute(CCommandParamParser *pcParser, uint32_t iRandomSeed) = 0;

	CString sGetName() { return s_name; };

protected:
	istream *ps_settings;
	CLog *pc_log;
	ostream *ps_summary;

private:
	CString s_name;
};//class CCommandExecution


class CReportDeceptive : public CCommandExecution
{
public:
	CReportDeceptive(istream *psSettings) : CCommandExecution("report_deceptive", psSettings, nullptr, nullptr) {};

	virtual CError eExecute(CCommandParamParser *pcParser, uint32_t iRandomSeed);
};

class CRandomSet : public CCommandExecution
{
public:
	CRandomSet(istream *psSettings, CLog *pcLog) : CCommandExecution("random_set", psSettings, pcLog, nullptr) {};

	virtual CError eExecute(CCommandParamParser *pcParser, uint32_t iRandomSeed);
};

class CAvisSubsetSumKnapsackProblemGenerator : public CCommandExecution
{
public:
	CAvisSubsetSumKnapsackProblemGenerator(CLog *pcLog, ostream *psSummary);

	virtual CError eExecute(CCommandParamParser *pcParser, uint32_t iRandomSeed);
};//class CAvisSubsetSumKnapsackProblemGenerator : public CCommandExecution


class CGenerateRandomMaxSat : public CCommandExecution
{
public:
	CGenerateRandomMaxSat(CLog *pcLog, ostream *psSummary);

	virtual CError eExecute(CCommandParamParser *pcParser, uint32_t iRandomSeed);
};//class CGenerateRandomMaxSat : public CCommandExecution


class CPlantedMaxSat2ProblemGeneration : public CCommandExecution
{
public:
	CPlantedMaxSat2ProblemGeneration(CLog *pcLog, ostream *psSummary);

	virtual CError eExecute(CCommandParamParser *pcParser, uint32_t iRandomSeed);
};//class CPlantedMaxSat2ProblemGeneration : public CCommandExecution


class CInjectedPopulationOptimizerRepeatabilityCheckingCommand : public CCommandExecution
{
public:
	CInjectedPopulationOptimizerRepeatabilityCheckingCommand(istream *psSettings, CLog *pcLog, ostream *psSummary);

	virtual CError eExecute(CCommandParamParser *pcParser, uint32_t iRandomSeed);
};//class CInjectedPopulationOptimizerRepeatabilityCheckingCommand : public CCommandExecution


class CChangeDetectionCommand : public CCommandExecution
{
public:
	CChangeDetectionCommand(istream *psSettings, CLog *pcLog, uint32_t iRandomSeed, ostream *psSummary);

	virtual CError eExecute(CCommandParamParser *pcParser, uint32_t iRandomSeed);

private:
	template <class TGenotype, class TFenotype>
	CError e_detect_changes();

	uint32_t i_random_seed;
};//class CChangeDetectionCommand : public CCommandExecution

#endif//COMMAND_EXECUTION_H