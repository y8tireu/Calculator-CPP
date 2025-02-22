# Qt Calculator

This is a simple **GUI calculator** written in **C++** using the **Qt** framework. It uses **QJSEngine** from Qt’s Qml modules to evaluate arithmetic expressions.

## Table of Contents

1. [Prerequisites](#prerequisites)  
2. [Downloading Qt](#downloading-qt)  
3. [Installing a C++ Compiler](#installing-a-c-compiler)  
4. [Verifying Qt Qml is Installed](#verifying-qt-qml-is-installed)  
5. [Building the Calculator](#building-the-calculator)  
   - [Using qmake](#using-qmake)  
   - [Using CMake](#using-cmake)  
6. [Running the Calculator](#running-the-calculator)  
7. [Full Code Sample](#full-code-sample)

---

## Prerequisites

- **Qt** (with Widgets and Qml modules)  
- A **C++ compiler** (e.g., GCC, Clang, or MSVC)  
- Build tools (e.g., `make`, `nmake`, or `mingw32-make`)

---

## Downloading Qt

### Option A: Official Qt Installer

1. Go to [https://www.qt.io/download](https://www.qt.io/download).  
2. Download the **Qt Online Installer** for your operating system.  
3. **During the installation**, ensure that you install/enable:
   - **Qt Base (Widgets)**  
   - **Qt Qml / Qt Declarative**  
   - **Qt Quick Controls** (optional if you only need Qml)  
   - A **compatible compiler toolchain** (e.g., MinGW, MSVC).  

### Option B: Package Manager (Linux or macOS)

On **Ubuntu/Debian**:
```bash
sudo apt-get update
sudo apt-get install qtbase5-dev qtdeclarative5-dev qt5-qmake
```

On **Fedora**:
```bash
sudo dnf install qt5-qtbase-devel qt5-qtdeclarative-devel
```

On **macOS** (Homebrew):
```bash
brew update
brew install qt@6
# For Qt5 instead:
# brew install qt@5
```
*(Adjust paths to use the installed `qmake` or `cmake`.)*

---

## Installing a C++ Compiler

- **Windows**:  
  - If you chose **MinGW** during Qt installation, it is included automatically.  
  - For **MSVC**, install **Visual Studio** or the **Build Tools for Visual Studio**.  
- **macOS**:  
  - Install **Xcode** or use the **Command Line Tools** (`xcode-select --install`) to get **clang**.  
- **Linux**:  
  - Typically GCC or Clang is already available. Otherwise:
    ```bash
    # Ubuntu/Debian
    sudo apt-get install build-essential
    
    # Fedora
    sudo dnf groupinstall "Development Tools"
    ```

---

## Verifying Qt Qml is Installed

1. **Open a terminal** (or Qt Command Prompt on Windows).  
2. Run:
   ```bash
   qmake -query | grep QT_INSTALL_QML
   ```
   If it returns a path, Qml is likely available.  

3. Alternatively, create a minimal `.pro` file with:
   ```pro
   TEMPLATE = app
   QT += qml
   ```
   Run `qmake`. If you do **not** see “Unknown module(s) in QT: qml,” the Qml module is installed.

---

## Building the Calculator

You can build the project with either **qmake** or **CMake**.

### Using qmake

1. Create a file named `calculator.pro` in the same folder as `main.cpp`:

   ```pro
   TEMPLATE = app
   CONFIG += c++11
   QT += widgets qml
   SOURCES += main.cpp
   ```

2. Run:
   ```bash
   qmake
   make
   ```
   On Windows (MinGW), use:
   ```bash
   qmake
   mingw32-make
   ```
   or on Windows (MSVC):
   ```bash
   qmake
   nmake
   ```

### Using CMake

1. Create a `CMakeLists.txt` file:
   ```cmake
   cmake_minimum_required(VERSION 3.5)
   project(QtCalculator LANGUAGES CXX)

   set(CMAKE_CXX_STANDARD 11)
   set(CMAKE_CXX_STANDARD_REQUIRED ON)

   find_package(Qt6 COMPONENTS Widgets Qml REQUIRED)
   # For Qt5: find_package(Qt5 COMPONENTS Widgets Qml REQUIRED)

   add_executable(QtCalculator main.cpp)
   target_link_libraries(QtCalculator
       Qt6::Widgets
       Qt6::Qml
   )
   # or Qt5::Widgets Qt5::Qml if using Qt5
   ```

2. From a terminal in the same directory:
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build .
   ```

---

## Running the Calculator

After building:

- **On Linux/macOS**:  
  Run `./calculator` (or `./QtCalculator`, depending on your chosen name) from the build directory.  

- **On Windows**:  
  Double-click `calculator.exe` (or run `calculator.exe` in a terminal).

You should see a **calculator window** with a display and buttons for digits and basic operators.

---

## Full Code Sample

Create a file named `main.cpp` in your project folder:

```cpp
/*****************************************************
 * A Simple GUI Calculator in C++ using Qt
 *****************************************************/

#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QGridLayout>
#include <QPushButton>
#include <QString>
#include <QObject>
#include <QJSEngine>
#include <QJSValue>

class Calculator : public QWidget
{
    Q_OBJECT  // Required for signals/slots

public:
    explicit Calculator(QWidget *parent = nullptr);

private slots:
    void handleButton();
    void evaluateExpression();

private:
    QLineEdit *display;   // Shows the current expression/result
    QString expression;   // Holds the typed expression
};

Calculator::Calculator(QWidget *parent)
    : QWidget(parent), display(new QLineEdit(this))
{
    // Layout to arrange buttons and display
    QGridLayout *layout = new QGridLayout(this);

    // Configure the display (non-editable for the user)
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    layout->addWidget(display, 0, 0, 1, 4);

    // Button labels in a grid
    QStringList buttons = {
        "7", "8", "9", "/",
        "4", "5", "6", "*",
        "1", "2", "3", "-",
        "0", "C", "=", "+"
    };

    int row = 1, col = 0;
    for (const QString &text : buttons) {
        QPushButton *btn = new QPushButton(text, this);
        btn->setFixedSize(50, 40);
        layout->addWidget(btn, row, col);
        col++;
        if (col > 3) {
            col = 0;
            row++;
        }
        connect(btn, &QPushButton::clicked, this, &Calculator::handleButton);
    }

    setLayout(layout);
    setWindowTitle("Cool Qt Calculator");
    setFixedSize(sizeHint());
}

void Calculator::handleButton()
{
    // Identify which button was clicked
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    QString btnText = btn->text();

    if (btnText == "C") {
        // Clear the expression and display
        expression.clear();
        display->clear();
    }
    else if (btnText == "=") {
        // Evaluate the expression
        evaluateExpression();
    }
    else {
        // Append the clicked character
        expression += btnText;
        display->setText(expression);
    }
}

void Calculator::evaluateExpression()
{
    QJSEngine engine;
    QJSValue result = engine.evaluate(expression);

    if (result.isError()) {
        // If there's an error, show "Error"
        display->setText("Error");
        expression.clear();
    } else {
        // Show the result
        QString resultStr = result.toString();
        display->setText(resultStr);
        expression = resultStr; // Keep the result as the new expression
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Calculator calc;
    calc.show();

    return app.exec();
}

#include "main.moc"
```

Make sure `calculator.pro` or `CMakeLists.txt` references `main.cpp` correctly, and ensure you have the Qt Qml module available. Then build, run, and enjoy your **Cool Qt Calculator**!
