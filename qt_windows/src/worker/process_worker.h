#pragma once

#include "pch.h"
#include <QThread>
#include "string_helper.h"

class ProcessWorker : public QObject {
Q_OBJECT

signals:

    void ProcessListReady(const std::list<std::pair<DWORD, win32::tstring>> &);

public slots:

    void DoGetProcessList();
};
