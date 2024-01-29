#include "main_window.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include <QFileDialog>
#include <utility>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        settings_("./setting.ini", QSettings::IniFormat)
{
    InitUi();
}

void MainWindow::InitUi()
{
    ui_.setupUi(this);
    setFixedSize(this->width(), this->height());
    ui_.table_process->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui_.table_process->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui_.lineEdit_dll->setText(settings_.value("DllPath").toString());
    connect(ui_.btn_refresh, &QPushButton::clicked, this, &MainWindow::OnRefreshClicked);
    connect(ui_.lineEdit_filter, &QLineEdit::textChanged, this, &MainWindow::UpdateProcessList);
    connect(ui_.table_process, &QTableView::doubleClicked, this, &MainWindow::OnProcessSelected);
    connect(ui_.btn_select_dll, &QPushButton::clicked, this, &MainWindow::OnSelectDllClicked);
}


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


void MainWindow::UpdateProcessList() const
{
    auto model = new QStandardItemModel();
    model->setColumnCount(2);
    model->setRowCount((int) process_list_.size());
    model->setHeaderData(0, Qt::Horizontal, "PID");
    model->setHeaderData(1, Qt::Horizontal, "线程");
    auto filter = ui_.lineEdit_filter->text()
            .replace(" ", "")
            .toLower()
            .toStdWString();
    int row = 0;
    for (auto &[pid, name]: process_list_) {
        std::wstring name_pattern(name);

        std::transform(name_pattern.begin(), name_pattern.end(), name_pattern.begin(), ::tolower);
        name_pattern.erase(std::remove(name_pattern.begin(), name_pattern.end(), L' '), name_pattern.end());
        if (!filter.empty() && name_pattern.find(filter) == std::wstring::npos) {
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
    ui_.table_process->scrollToTop();
}

void MainWindow::GetProcessList(std::list<std::pair<DWORD, win32::tstring>> process_list)
{
    process_list_ = std::move(process_list);
    UpdateProcessList();
}

void MainWindow::OnProcessSelected(const QModelIndex &index) const
{
    auto pid_index = ui_.table_process->model()->index(index.row(), 0);
    auto name_index = ui_.table_process->model()->index(index.row(), 1);
    auto pid = ui_.table_process->model()->data(pid_index).toInt();
    auto name = ui_.table_process->model()->data(name_index).toString();

    ui_.label_pid->setText(QString("PID: %1").arg(pid));
    ui_.lineEdit_process->setText(name);
}

void MainWindow::OnSelectDllClicked()
{
    auto lastPath = settings_.value("DllPath").toString();
    auto dialog = QFileDialog::getOpenFileName((QWidget *) this, "选择DLL", lastPath, "DLL文件 (*.dll)");
    if (dialog.isEmpty()) {
        return;
    }
    ui_.lineEdit_dll->setText(dialog);
    settings_.setValue("DllPath", dialog);
}


MainWindow::~MainWindow() = default;
