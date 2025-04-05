#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDockWidget>
#include <QToolBar>
#include <QLabel>
#include <QComboBox>
#include <QGroupBox>
#include <QStatusBar>
#include <QSplitter> // To allow resizing between chat list and main area

// Forward declarations
QT_BEGIN_NAMESPACE
class QAction;
class QToolButton;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onChatSelectionChanged(); // Example slot

private:
    void setupUi();
    void createActions();
    void createToolBars();
    void createDockWindows();
    void createCentralChatWidget();
    void createStatusBar();
    void loadDummyData();
    void applyBasicStyling(); // Basic dark theme simulation

    // --- Actions ---
    QAction *chatAction;
    QAction *exploreAction; // Placeholder for second icon
    QAction *folderAction;
    QAction *searchAction;
    QAction *newChatAction; // '+' button

    // --- Toolbars ---
    QToolBar *mainToolBar; // For top icons/controls
    QToolBar *sideToolBar; // For left-side main navigation icons

    // --- Dock Widgets ---
    QDockWidget *chatListDock;
    QDockWidget *configDock;

    // --- Widgets inside Docks ---
    // Left Chat List
    QListWidget *chatListWidget;

    // Right Config Panel
    QWidget *configWidgetContents; // Container for config widgets
    QVBoxLayout *configLayout;
    QLabel *presetLabel;
    QComboBox *presetComboBox;
    QPushButton *commitButton;
    QPushButton *saveAsNewButton;
    QGroupBox *systemPromptGroup;
    QTextEdit *systemPromptEdit;
    QGroupBox *settingsGroup; // Placeholder for settings sections
    QGroupBox *samplingGroup;
    QGroupBox *outputGroup;
    QGroupBox *speculativeGroup;

    // --- Central Widget ---
    QWidget *centralChatWidget;
    QVBoxLayout *centralChatLayout;
    QTextEdit *chatHistoryDisplay;
    QWidget *inputAreaWidget;
    QHBoxLayout *inputAreaLayout;
    QLineEdit *messageInput;
    QToolButton *attachButton; // Placeholder for paperclip
    QPushButton *sendButton;

    // --- Status Bar ---
    QLabel *statusContextLabel;
    QLabel *statusResourcesLabel;

    // --- Model Selection (Example) ---
    QComboBox *modelComboBox;
};

#endif // MAINWINDOW_H