#pragma once

#include <Qt>
#include "ui_main_window.h"
#include "Windows.h"
#include "string_helper.h"
#include "../worker/process_worker.h"


class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

public slots:

    void UpdateProcessList();

    void GetProcessList(std::list<std::pair<DWORD, win32::tstring>> process_list);

    void OnRefreshClicked() const;

private:
    Ui::MainWindow ui_{};

    std::list<std::pair<DWORD, win32::tstring>> process_list_{};
};