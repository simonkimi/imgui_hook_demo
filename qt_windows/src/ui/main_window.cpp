#include "main_window.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include <QFileDialog>
#include <utility>
#include "process_helper.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        settings_("./setting.ini", QSettings::IniFormat)
{
    InitUi();
    OnRefreshClicked();
}

void MainWindow::InitUi()
{
    ui_.setupUi(this);
    setFixedSize(this->width(), this->height());
    ui_.table_process->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui_.table_process->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui_.lineEdit_dll->setText(settings_.value("DllPath").toString());
    ui_.lineEdit_dll->setClearButtonEnabled(true);
    ui_.lineEdit_filter->setText(settings_.value("Filter").toString());
    ui_.lineEdit_filter->setClearButtonEnabled(true);
    ui_.lineEdit_process->setText(settings_.value("ProcessName").toString());
    connect(ui_.btn_refresh, &QPushButton::clicked, this, &MainWindow::OnRefreshClicked);
    connect(ui_.lineEdit_filter, &QLineEdit::textChanged, this, &MainWindow::OnFilterChanged);
    connect(ui_.table_process, &QTableView::doubleClicked, this, &MainWindow::OnProcessSelected);
    connect(ui_.btn_select_dll, &QPushButton::clicked, this, &MainWindow::OnSelectDllClicked);
    connect(ui_.btn_inject, &QPushButton::clicked, this, &MainWindow::OnInjectDllClicked);
    connect(ui_.btn_free, &QPushButton::clicked, this, &MainWindow::OnFreeDllClicked);
    connect(ui_.btn_test, &QPushButton::clicked, this, &MainWindow::OnTestClicked);
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

void MainWindow::OnProcessSelected(const QModelIndex &index)
{
    auto pid_index = ui_.table_process->model()->index(index.row(), 0);
    auto name_index = ui_.table_process->model()->index(index.row(), 1);
    auto pid = ui_.table_process->model()->data(pid_index).toInt();
    auto name = ui_.table_process->model()->data(name_index).toString();

    ui_.label_pid->setText(QString("PID: %1").arg(pid));
    ui_.lineEdit_process->setText(name);
    settings_.setValue("ProcessName", name);
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

void MainWindow::OnInjectDllClicked() const
{
    auto process_name = ui_.lineEdit_process->text().toStdWString();
    auto pid = win32::FindProcessById(process_name.c_str());
    if (pid <= 0) {
        QMessageBox::critical((QWidget *) this, "提示", "进程不存在");
        return;
    }
    ui_.label_pid->setText(QString("PID: %1").arg(pid));
    auto dll_path = ui_.lineEdit_dll->text().replace("/", "\\").toStdWString();
    auto result = win32::CrtInjectDll(pid, dll_path.c_str());
    if (result) {
        QMessageBox::information((QWidget *) this, "提示", "注入成功");
    } else {
        QMessageBox::critical((QWidget *) this, "提示", "注入失败");
    }
}

void MainWindow::OnFreeDllClicked() const
{
    auto pid = ui_.label_pid->text().split(":")[1].toInt();
    auto dll_path = ui_.lineEdit_dll->text().replace("/", "\\").toStdWString();
    auto result = win32::CrtFreeDll(pid, dll_path.c_str());
    if (result) {
        QMessageBox::information((QWidget *) this, "提示", "卸载成功");
    } else {
        QMessageBox::critical((QWidget *) this, "提示", "卸载失败");
    }
}

void MainWindow::OnFilterChanged()
{
    settings_.setValue("Filter", ui_.lineEdit_filter->text());
    UpdateProcessList();
}

void MainWindow::OnTestClicked() const
{
    MessageBox((HWND) winId(), _T("我还没有改变"), _T("测试"), MB_OK);
}


MainWindow::~MainWindow() = default;
