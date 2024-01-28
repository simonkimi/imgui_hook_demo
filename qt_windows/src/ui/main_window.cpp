#include "main_window.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include <utility>


void MainWindow::OnRefreshClicked() const
{
    auto thread = new QThread();
    auto worker = new ProcessWorker();
    worker->moveToThread(thread);
    connect(thread, &QThread::started, worker, &ProcessWorker::DoGetProcessList);
    connect(worker, &ProcessWorker::ProcessListReady, this, &MainWindow::GetProcessList);
    connect(worker, &ProcessWorker::ProcessListReady, worker, &ProcessWorker::deleteLater);
    connect(worker, &ProcessWorker::ProcessListReady, thread, &QThread::quit);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start();
}


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    ui_.setupUi(this);
    connect(ui_.btn_refresh, &QPushButton::clicked, this, &MainWindow::OnRefreshClicked);
    connect(ui_.lineEdit_filter, &QLineEdit::textChanged, this, &MainWindow::UpdateProcessList);
}

void MainWindow::UpdateProcessList()
{
    auto model = new QStandardItemModel();
    model->setColumnCount(2);
    model->setRowCount((int) process_list_.size());
    model->setHeaderData(0, Qt::Horizontal, "PID");
    model->setHeaderData(1, Qt::Horizontal, "线程");
    auto filter = ui_.lineEdit_filter->text().toStdWString();
    int row = 0;
    for (auto &[pid, name]: process_list_) {
        if (!filter.empty() && name.find(filter) == std::wstring::npos) {
            continue;
        }
        auto pid_item = new QStandardItem(QString::number(pid));
        auto name_item = new QStandardItem(QString::fromStdWString(name));
        model->setItem(row, 0, pid_item);
        model->setItem(row, 1, name_item);
        row++;
    }
    ui_.table_process->setModel(model);
    ui_.table_process->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui_.table_process->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui_.table_process->verticalHeader()->setVisible(false);
}

void MainWindow::GetProcessList(std::list<std::pair<DWORD, win32::tstring>> process_list)
{
    process_list_ = std::move(process_list);
    UpdateProcessList();
}

MainWindow::~MainWindow() = default;
