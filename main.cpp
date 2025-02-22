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

    // Button labels in a grid fashion
    // 7 8 9 /
    // 4 5 6 *
    // 1 2 3 -
    // 0 C = +
    QStringList buttons = {
        "7", "8", "9", "/",
        "4", "5", "6", "*",
        "1", "2", "3", "-",
        "0", "C", "=", "+"
    };

    int row = 1, col = 0;
    for (const QString &text : buttons) {
        QPushButton *btn = new QPushButton(text, this);
        btn->setFixedSize(50, 40); // (Optional) Make buttons a bit larger
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
        // Clear the expression and the display
        expression.clear();
        display->clear();
    }
    else if (btnText == "=") {
        // Evaluate the expression
        evaluateExpression();
    }
    else {
        // Append the clicked character to our expression
        expression += btnText;
        display->setText(expression);
    }
}

void Calculator::evaluateExpression()
{
    QJSEngine engine;
    QJSValue result = engine.evaluate(expression);

    if (result.isError()) {
        // If there was a parsing or runtime error, show 'Error'
        display->setText("Error");
        expression.clear();
    } else {
        // Show the result, and store it as the new expression
        QString resultStr = result.toString();
        display->setText(resultStr);
        expression = resultStr;
    }
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Calculator calc;
    calc.show();

    return app.exec();
}

#include "main.moc"  // Ensures Qt’s MOC (Meta-Object Compiler) processes this file

