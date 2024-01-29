#pragma once

#include "pch.h"
#include "ui_main_window.h"
#include "string_helper.h"
#include "../worker/process_worker.h"
#include <QSettings>



class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;
    
    void InitUi();

public slots:

    void UpdateProcessList() const;

    void OnProcessSelected(const QModelIndex &index) const;

    void GetProcessList(std::list<std::pair<DWORD, win32::tstring>> process_list);

    void OnRefreshClicked() const;
    
    void OnSelectDllClicked();
    
    void OnInjectDllClicked() const;
    
    void OnFreeDllClicked() const;
    
    void OnFilterChanged();

private:
    Ui::MainWindow ui_{};

    std::list<std::pair<DWORD, win32::tstring>> process_list_{};
    
    QSettings settings_;
};