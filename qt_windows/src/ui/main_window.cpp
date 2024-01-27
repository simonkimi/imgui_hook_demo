#include "main_window.h"
#include "process_helper.h"
#include <QMessageBox>
#include <QStandardItemModel>


void MainWindow::OnRefreshClicked() const {
    auto thread = new QThread();
    auto worker = new ProcessWorker();
    worker->moveToThread(thread);
    connect(thread, &QThread::started, worker, &ProcessWorker::DoGetProcessList);
    connect(worker, &ProcessWorker::ProcessListReady, this, &MainWindow::UpdateProcessList);
    connect(worker, &ProcessWorker::ProcessListReady, worker, &ProcessWorker::deleteLater);
    connect(worker, &ProcessWorker::ProcessListReady, thread, &QThread::quit);
    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    thread->start();
}


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    ui_.setupUi(this);
    connect(ui_.btn_refresh, &QPushButton::clicked, this, &MainWindow::OnRefreshClicked);
}

void MainWindow::UpdateProcessList(const std::list<std::pair<DWORD, win32::tstring>> &process_list) {
    auto model = new QStandardItemModel();
    model->setColumnCount(2);
    model->setRowCount((int) process_list.size());
    model->setHeaderData(0, Qt::Horizontal, "PID");
    model->setHeaderData(1, Qt::Horizontal, "线程");
    int row = 0;
    for (auto &[pid, name]: process_list) {
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

MainWindow::~MainWindow() = default;
