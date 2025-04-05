#include "mainwindow.h"
#include <QApplication> // For styling
#include <QStyleFactory> // For styling
#include <QPalette> // For styling
#include <QIcon>
#include <QGroupBox>
#include <QSpacerItem>
#include <QSplitter>
#include <QToolButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi();
    loadDummyData();
    applyBasicStyling(); // Apply basic dark theme
    setWindowTitle("LM Studio Clone - Qt Demo");
    setMinimumSize(800, 600); // Set a reasonable minimum size
}

MainWindow::~MainWindow()
{
    // Qt's parent-child relationship handles deletion of most widgets
}

void MainWindow::setupUi()
{
    createActions();
    createToolBars(); // Create toolbars first
    createDockWindows(); // Create docks before central widget if they contain main elements
    createCentralChatWidget(); // Create the main chat area
    createStatusBar();

    // Add docks to the main window
    addDockWidget(Qt::LeftDockWidgetArea, chatListDock);
    addDockWidget(Qt::RightDockWidgetArea, configDock);

    // Set the central widget *after* docks if you want docks around it
    setCentralWidget(centralChatWidget);

    // Optional: Prevent docks from being floatable or closable for a fixed layout
    chatListDock->setFeatures(QDockWidget::DockWidgetMovable); // Only movable
    configDock->setFeatures(QDockWidget::DockWidgetMovable); // Only movable

    // Set initial sizes (adjust as needed)
    chatListDock->setMinimumWidth(180);
    configDock->setMinimumWidth(280);
    resize(1200, 800); // Set a default size
}

void MainWindow::createActions()
{
    // Use QIcon::fromTheme for standard icons, or provide paths to your own icon files
    // Example: QIcon(":/icons/my_chat_icon.png")
    chatAction = new QAction(QIcon::fromTheme("document-properties"), tr("&Chats"), this);
    exploreAction = new QAction(QIcon::fromTheme("document-open-recent"), tr("&Explore"), this); // Placeholder
    folderAction = new QAction(QIcon::fromTheme("folder"), tr("&Local Server"), this); // Placeholder mapping
    searchAction = new QAction(QIcon::fromTheme("edit-find"), tr("&Search"), this);
    newChatAction = new QAction(QIcon::fromTheme("document-new"), tr("&New Chat"), this);
}

void MainWindow::createToolBars()
{
    // --- Top Toolbar (Example) ---
    mainToolBar = addToolBar(tr("Main"));
    mainToolBar->setObjectName("mainToolBar"); // For styling
    mainToolBar->setMovable(false);

    QLabel *modelLabel = new QLabel("  Model: "); // Add some spacing
    modelComboBox = new QComboBox(this);
    modelComboBox->addItem("meta-llama-3.1-8B-Instruct");
    modelComboBox->addItem("other-model-example");
    modelComboBox->setMinimumWidth(200);

    mainToolBar->addWidget(modelLabel);
    mainToolBar->addWidget(modelComboBox);
    mainToolBar->addSeparator(); // Spacer
    mainToolBar->addAction(new QAction(QIcon::fromTheme("media-eject"), "Eject", this)); // Example button
    mainToolBar->addAction(new QAction(QIcon::fromTheme("preferences-desktop-theme"), "Appearance", this));
    mainToolBar->addAction(new QAction(QIcon::fromTheme("edit-clear"), "Clear All", this));
    mainToolBar->addAction(new QAction(QIcon::fromTheme("edit-copy"), "Duplicate", this)); // Using copy icon as placeholder


    // --- Left Sidebar Toolbar ---
    sideToolBar = new QToolBar(tr("Navigation"));
    sideToolBar->setObjectName("sideToolBar"); // For styling
    sideToolBar->setOrientation(Qt::Vertical);
    sideToolBar->setIconSize(QSize(32, 32)); // Adjust icon size
    sideToolBar->setMovable(false);
    sideToolBar->setFloatable(false);

    sideToolBar->addAction(chatAction);
    sideToolBar->addAction(exploreAction);
    sideToolBar->addAction(folderAction);
    sideToolBar->addAction(searchAction);

    addToolBar(Qt::LeftToolBarArea, sideToolBar);
}


void MainWindow::createDockWindows()
{
    // --- Left Chat List Dock ---
    chatListDock = new QDockWidget(tr("Chats"), this);
    chatListDock->setObjectName("chatListDock"); // Important for QSS styling
    chatListDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QWidget *chatListContents = new QWidget;
    QVBoxLayout *chatListLayout = new QVBoxLayout(chatListContents);
    chatListLayout->setContentsMargins(0, 5, 0, 0); // Adjust margins

    // Add a 'New Chat' button or similar at the top if needed
    QPushButton *newChatButton = new QPushButton(QIcon::fromTheme("list-add"), " New Chat");
    chatListLayout->addWidget(newChatButton); // Example button

    chatListWidget = new QListWidget(chatListDock);
    chatListWidget->setObjectName("chatListWidget");
    connect(chatListWidget, &QListWidget::currentItemChanged, this, &MainWindow::onChatSelectionChanged);
    chatListLayout->addWidget(chatListWidget);

    chatListDock->setWidget(chatListContents);


    // --- Right Configuration Dock ---
    configDock = new QDockWidget(tr("Advanced Configuration"), this);
    configDock->setObjectName("configDock"); // For styling
    configDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    configWidgetContents = new QWidget;
    configLayout = new QVBoxLayout(configWidgetContents);
    configLayout->setSpacing(10); // Spacing between elements

    // Preset Section
    QHBoxLayout* presetLayout = new QHBoxLayout();
    presetLabel = new QLabel(tr("Preset:"));
    presetComboBox = new QComboBox();
    presetComboBox->addItem("Coding Helper (C++)");
    presetComboBox->addItem("Default");
    presetComboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed); // Make combo box expand
    QPushButton *discardButton = new QPushButton(tr("Discard Unsaved")); // Example button
    presetLayout->addWidget(presetLabel);
    presetLayout->addWidget(presetComboBox);
    presetLayout->addWidget(discardButton);
    configLayout->addLayout(presetLayout);


    // Action Buttons
    QHBoxLayout* configActionLayout = new QHBoxLayout();
    commitButton = new QPushButton(QIcon::fromTheme("document-save"), tr("Commit Changes"));
    saveAsNewButton = new QPushButton(QIcon::fromTheme("save-as"), tr("Save As New..."));
    configActionLayout->addWidget(commitButton);
    configActionLayout->addWidget(saveAsNewButton);
    configActionLayout->addStretch(); // Push buttons to the left
    configLayout->addLayout(configActionLayout);


    // System Prompt Section
    systemPromptGroup = new QGroupBox(tr("System Prompt"));
    QVBoxLayout *promptLayout = new QVBoxLayout(systemPromptGroup);
    systemPromptEdit = new QTextEdit();
    systemPromptEdit->setPlaceholderText("You are an incredibly good C++ engineer...");
    systemPromptEdit->setMaximumHeight(100); // Limit height
    promptLayout->addWidget(systemPromptEdit);
    configLayout->addWidget(systemPromptGroup);

    // Placeholder sections for other settings
    settingsGroup = new QGroupBox(tr("Settings"));
    // Add placeholder content or actual widgets here if needed
    settingsGroup->setLayout(new QVBoxLayout()); // Needs a layout
    settingsGroup->layout()->addWidget(new QLabel("Setting options..."));
    configLayout->addWidget(settingsGroup);

    samplingGroup = new QGroupBox(tr("Sampling"));
    samplingGroup->setLayout(new QVBoxLayout());
    samplingGroup->layout()->addWidget(new QLabel("Sampling options..."));
    configLayout->addWidget(samplingGroup);

    outputGroup = new QGroupBox(tr("Structured Output"));
     outputGroup->setLayout(new QVBoxLayout());
    outputGroup->layout()->addWidget(new QLabel("Output options..."));
    configLayout->addWidget(outputGroup);

    speculativeGroup = new QGroupBox(tr("Speculative Decoding"));
    speculativeGroup->setLayout(new QVBoxLayout());
    speculativeGroup->layout()->addWidget(new QLabel("Decoding options..."));
    configLayout->addWidget(speculativeGroup);


    configLayout->addStretch(); // Pushes content to the top

    configWidgetContents->setLayout(configLayout); // Set the layout on the container
    configDock->setWidget(configWidgetContents); // Set the container as the dock's widget
}

void MainWindow::createCentralChatWidget()
{
    centralChatWidget = new QWidget(this);
    centralChatLayout = new QVBoxLayout(centralChatWidget);
    centralChatLayout->setContentsMargins(5, 5, 5, 5); // Reduce margins
    centralChatLayout->setSpacing(5); // Reduce spacing

    // Chat History Display
    chatHistoryDisplay = new QTextEdit(this);
    chatHistoryDisplay->setObjectName("chatHistoryDisplay"); // For styling
    chatHistoryDisplay->setReadOnly(true);
    // Use chatHistoryDisplay->append() or ->setHtml() to add messages
    centralChatLayout->addWidget(chatHistoryDisplay, 1); // Give it stretch factor 1

    // Input Area
    inputAreaWidget = new QWidget(this);
    inputAreaLayout = new QHBoxLayout(inputAreaWidget);
    inputAreaLayout->setContentsMargins(0, 0, 0, 0); // No margins for this inner layout
    inputAreaLayout->setSpacing(5);

    messageInput = new QLineEdit(this);
    messageInput->setPlaceholderText("Type a message and press Enter to send ...");
    messageInput->setObjectName("messageInput"); // For styling

    attachButton = new QToolButton(this);
    attachButton->setIcon(QIcon::fromTheme("mail-attachment")); // Placeholder icon
    attachButton->setToolTip("Attach file (placeholder)");

    sendButton = new QPushButton(QIcon::fromTheme("mail-send"), ""); // Icon only, maybe text " Send"
    sendButton->setToolTip("Send Message");

    inputAreaLayout->addWidget(messageInput, 1); // Input stretches
    inputAreaLayout->addWidget(attachButton);
    inputAreaLayout->addWidget(sendButton);

    centralChatLayout->addWidget(inputAreaWidget); // Add input area below history

    // Optional: Add a splitter if you want the chat list and chat area resizeable relative to each other
    // QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    // splitter->addWidget(chatListDock); // Need to rethink how docks are added if using splitter like this
    // splitter->addWidget(centralChatWidget);
    // setCentralWidget(splitter); // Instead of setCentralWidget(centralChatWidget)
    // // You would likely need to manage docks differently if using a splitter as the central widget.
    // // For simplicity, this example keeps them as standard dock widgets.
}


void MainWindow::createStatusBar()
{
    statusBar()->setObjectName("statusBar"); // For styling

    // Left side status
    QLabel* statusAppLabel = new QLabel(" LM Studio 0.3.11 (Build 1) ");
    statusBar()->addWidget(statusAppLabel);

    // Spacer or permanent widget to push others to the right
    QLabel* spacer = new QLabel();
    statusBar()->addWidget(spacer, 1); // Stretchy spacer

    // Right side status elements
    statusContextLabel = new QLabel(" Context: 6.7% full ");
    statusResourcesLabel = new QLabel(" RAM: 1.18 GB | CPU: 0.00 % ");
    QPushButton* settingsButton = new QPushButton(QIcon::fromTheme("preferences-system"), ""); // Settings gear
    settingsButton->setFlat(true); // Make it look less like a button

    statusBar()->addPermanentWidget(statusContextLabel);
    statusBar()->addPermanentWidget(statusResourcesLabel);
    statusBar()->addPermanentWidget(settingsButton);
}

void MainWindow::loadDummyData()
{
    // Populate Chat List
    chatListWidget->addItem("Secret project");
    QListWidgetItem* currentItem = new QListWidgetItem("C++ Simple File System");
    chatListWidget->addItem(currentItem);
    chatListWidget->addItem("Financial analysis");
    chatListWidget->addItem("log about version of ...");
    chatListWidget->setCurrentItem(currentItem); // Select one item

    // Populate Chat History (Example using HTML for basic formatting)
    chatHistoryDisplay->setHtml(
        "<p><b>Assistant</b> <small style='color: gray;'>meta-llama-3.1-8B-Instruct</small></p>"
        "<p>Before we begin, let's outline the basic components of our file system:</p>"
        "<ol>"
        "<li><b>FileSystem:</b> This will be the top-level class responsible for managing the file system.</li>"
        "<li><b>Directory:</b> Represents a directory in the file system. It contains a map of child directories and files.</li>"
        "<li><b>File:</b> Represents a file in the file system. It stores the file's name, size, and contents.</li>"
        "</ol>"
        "<p><b>Implementation</b></p>"
        "<pre style='background-color: #2c3e50; color: #ecf0f1; padding: 10px; border-radius: 4px;'>" // Basic code block styling
        "#include <iostream>\n"
        "#include <fstream>\n"
        "#include <string>\n"
        "#include <map>\n\n"
        "// Forward declarations\n"
        "class FileSystem;\n"
        "class Directory;\n"
        "class File;\n\n"
        "// Abstract base class for File System components (Directory/File)\n"
        "class FileSystemComponent {\n"
        "public:\n"
        "    virtual ~FileSystemComponent() {}\n"
        "};"
        "</pre>"
    );

    // Populate System Prompt
    systemPromptEdit->setText(
        "You are an incredibly good C++ engineer. Think about the problems you're about to solve step-by-step. First make a plan, and then ask user to confirm. Only then act on it."
    );
}

// Example Slot Implementation
void MainWindow::onChatSelectionChanged()
{
    QListWidgetItem *item = chatListWidget->currentItem();
    if (item) {
        // In a real app, load the actual chat history for item->text()
        chatHistoryDisplay->setPlainText(QString("Chat history for: %1\n\n(Load actual content here)").arg(item->text()));
        messageInput->setFocus(); // Focus input when chat changes
    } else {
        chatHistoryDisplay->clear();
    }
}

void MainWindow::applyBasicStyling()
{
    // Use Fusion style for a more modern look than default Windows/macOS styles
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    // Basic Dark Theme Palette
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(35, 35, 35)); // Background for text entry widgets
    darkPalette.setColor(QPalette::AlternateBase, QColor(53, 53, 53)); // Used in list/table views
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53, 53, 53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red); // Often used for highlighting
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218)); // Selection color
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    // Disabled state colors
    darkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(127, 127, 127));
    darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(127, 127, 127));
    darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(127, 127, 127));


    QApplication::setPalette(darkPalette);

    // Optional: Apply some basic QSS for finer control (Example)
    // This is where you'd do the detailed styling matching the screenshot
    // qApp->setStyleSheet("QMainWindow { background-color: #2d2d2d; } "
    //                     "QDockWidget { titlebar-close-icon: url(close.png); titlebar-normal-icon: url(undock.png); }" // Needs icons
    //                     "#chatListWidget { background-color: #3a3a3a; border: none; }"
    //                     "#chatHistoryDisplay { background-color: #303030; border: 1px solid #444; }"
    //                     "QPushButton { background-color: #4a4a4a; border: 1px solid #555; padding: 5px; } QPushButton:hover { background-color: #5a5a5a; } "
    //                     );
}