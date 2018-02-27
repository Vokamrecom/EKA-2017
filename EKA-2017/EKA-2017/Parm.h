#pragma once
#define PARM_IN  L"-in:"
#define PARM_LOG L"-log:"
#define PARMIT	 L"-IT"
#define PARMLT	 L"-LT"
#define PARMRUL	 L"-R"
#define PARM_MAX_SIZE 300
#define PARM_LOG_DEFAULT_EXT L".log"

namespace Parm
{
	struct PARM
	{
		wchar_t in[PARM_MAX_SIZE];
		wchar_t log[PARM_MAX_SIZE];
		wchar_t lt[PARM_MAX_SIZE];
		wchar_t it[PARM_MAX_SIZE];
		bool LTtable;
		bool ITtable;
		bool Rules;
	};

	PARM getparm(int argc, wchar_t* argv[]);
}
