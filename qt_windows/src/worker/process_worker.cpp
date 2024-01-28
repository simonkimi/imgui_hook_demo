#include "process_worker.h"
#include "process_helper.h"


void ProcessWorker::DoGetProcessList()
{
    auto processList = win32::GetProcessList();
    emit ProcessListReady(processList);
}

