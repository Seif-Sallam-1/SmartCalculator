#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QWheelEvent>
#include <QRegularExpressionValidator>
#include "Parser.h"

// --- 1. Custom View ---
class ZoomableView : public QGraphicsView {
public:
    ZoomableView(QGraphicsScene *scene) : QGraphicsView(scene) {
        setRenderHint(QPainter::Antialiasing);
        setDragMode(QGraphicsView::ScrollHandDrag);
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        viewport()->setCursor(Qt::CrossCursor);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    }
protected:
    void wheelEvent(QWheelEvent *event) override {
        double scaleFactor = 1.15;
        if (event->angleDelta().y() > 0) scale(scaleFactor, scaleFactor);
        else scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
};

// --- 2. Custom Button ---
class CalcButton : public QPushButton {
public:
    CalcButton(const QString &text, const QString &color = "#2D2D2D") : QPushButton(text) {
        setFixedSize(60, 60);
        setStyleSheet("QPushButton { "
                      "  background-color: " + color + ";"
                      "  color: white; "
                      "  border-radius: 30px; "
                      "  font-size: 14px; " // Slightly smaller font to fit "log_b"
                      "  font-family: Arial; "
                      "  font-weight: bold;"
                      "}"
                      "QPushButton:pressed { background-color: #505050; }");
    }
};

// --- 3. Main Window ---
class MainWindow : public QMainWindow {
    Q_OBJECT

    QLineEdit *display;
    QGraphicsScene *scene;
    ZoomableView *view;
    SymbolTable vars;

public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent) {
        setWindowTitle("Smart Structure Calculator");
        resize(550, 950);
        setStyleSheet("QMainWindow { background-color: #121212; }");

        QWidget *centralWidget = new QWidget;
        setCentralWidget(centralWidget);

        QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
        mainLayout->setSpacing(10);
        mainLayout->setContentsMargins(10, 10, 10, 10);

        // --- DISPLAY ---
        display = new QLineEdit;
        display->setPlaceholderText("Type expression or 'x+1=5'...");
        display->setAlignment(Qt::AlignRight);
        display->setStyleSheet("QLineEdit { "
                               "  background-color: #1E1E1E; "
                               "  color: white; "
                               "  font-size: 32px; "
                               "  border: 1px solid #333; "
                               "  border-radius: 10px;"
                               "  padding: 10px;"
                               "}");

        QRegularExpression regex("[0-9+\\-*/^().=acegilnopqrstx_]+");
        QRegularExpressionValidator *validator = new QRegularExpressionValidator(regex, this);
        display->setValidator(validator);

        mainLayout->addWidget(display);

        // --- TREE VIEW ---
        scene = new QGraphicsScene;
        view = new ZoomableView(scene);
        view->setStyleSheet("border: none; background-color: #1E1E1E; border-radius: 15px;");
        mainLayout->addWidget(view);

        // --- KEYPAD ---
        QWidget *keypadWidget = new QWidget;
        QGridLayout *gridLayout = new QGridLayout(keypadWidget);
        gridLayout->setSpacing(10);
        gridLayout->setContentsMargins(0, 0, 0, 0);

        // Define Buttons
        struct BtnDef { QString label; QString color; };

        // ADDED: "Eq" (inserts =) and "log_b" (inserts base log template)
        QList<QList<BtnDef>> rows = {
            {{"AC", "#D32F2F"}, {"DEL", "#D32F2F"}, {"(", "#4E4E4E"}, {")", "#4E4E4E"}, {"^", "#4B5EFC"}},
            {{"sin", "#2D2D2D"}, {"cos", "#2D2D2D"}, {"tan", "#2D2D2D"}, {"Eq", "#9C27B0"}, {"/", "#4B5EFC"}}, // Eq is Purple
            {{"log", "#2D2D2D"}, {"log_b", "#2D2D2D"}, {"sqrt", "#2D2D2D"}, {"x", "#2D2D2D"}, {"*", "#4B5EFC"}}, // New logs
            {{"7", "#2D2D2D"}, {"8", "#2D2D2D"}, {"9", "#2D2D2D"}, {"pi", "#2D2D2D"}, {"-", "#4B5EFC"}},
            {{"4", "#2D2D2D"}, {"5", "#2D2D2D"}, {"6", "#2D2D2D"}, {"e", "#2D2D2D"}, {"+", "#4B5EFC"}},
            {{"1", "#2D2D2D"}, {"2", "#2D2D2D"}, {"3", "#2D2D2D"}, {".", "#2D2D2D"}, {"=", "#388E3C"}},
            {{"0", "#2D2D2D"}} // 0 spans wide
        };

        int r = 0;
        for (const auto& row : rows) {
            int c = 0;
            for (const auto& btnDef : row) {
                QString text = btnDef.label;
                QString color = btnDef.color;

                CalcButton *btn = new CalcButton(text, color);

                // Special Sizing
                if (text == "=") {
                     // The Equals button is vertical in the corner
                     btn->setFixedSize(60, 60);
                }
                else if (text == "0") {
                    btn->setFixedSize(130, 60); // Wide 0
                }

                // Grid Placement Logic
                if (text == "0") gridLayout->addWidget(btn, r, c, 1, 2);
                else gridLayout->addWidget(btn, r, c);

                // --- BUTTON LOGIC ---
                if (text == "AC") {
                    connect(btn, &QPushButton::clicked, this, [this](){
                        display->clear(); scene->clear(); display->setFocus();
                    });
                }
                else if (text == "DEL") {
                    connect(btn, &QPushButton::clicked, this, [this](){
                        display->backspace(); display->setFocus();
                    });
                }
                else if (text == "=") {
                    connect(btn, &QPushButton::clicked, this, &MainWindow::onEqualPressed);
                }
                else if (text == "Eq") {
                    // NEW: Inserts the "=" symbol for equations
                    connect(btn, &QPushButton::clicked, this, [this](){
                        display->insert("="); display->setFocus();
                    });
                }
                else if (text == "log") {
                    // FIX: Inserts "log(" automatically
                    connect(btn, &QPushButton::clicked, this, [this](){
                        display->insert("log("); display->setFocus();
                    });
                }
                else if (text == "log_b") {
                    // NEW: Inserts Change of Base Template
                    connect(btn, &QPushButton::clicked, this, [this](){
                        display->insert("log()/log()");
                        // Move cursor back to inside the first parens
                        display->setCursorPosition(display->cursorPosition() - 7);
                        display->setFocus();
                    });
                }
                else if (text == "sin" || text == "cos" || text == "tan" || text == "sqrt") {
                    // Auto-add parenthesis for functions
                    connect(btn, &QPushButton::clicked, this, [this, text](){
                        display->insert(text + "("); display->setFocus();
                    });
                }
                else {
                    // Normal Insert
                    connect(btn, &QPushButton::clicked, this, [this, text](){
                        display->insert(text); display->setFocus();
                    });
                }
                c++;
            }
            r++;
        }

        keypadWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        mainLayout->addWidget(keypadWidget);

        vars["pi"] = 3.1415926535;
        vars["e"] = 2.7182818284;
    }

private slots:
    void onEqualPressed() {
        std::string text = display->text().toStdString();
        if (text.empty()) return;

        // Auto-fix: Append closing parenthesis if missing
        int openP = std::count(text.begin(), text.end(), '(');
        int closeP = std::count(text.begin(), text.end(), ')');
        while (openP > closeP) {
            text += ")";
            closeP++;
        }

        // Detect Solver Mode
        if (text.find('=') != std::string::npos) {
            solveEquation(text);
        } else {
            calculateExpression(text);
        }
    }

    void calculateExpression(std::string text) {
        try {
            if (vars.find("x") == vars.end()) vars["x"] = 0;

            Lexer lexer(text);
            auto tokens = lexer.tokenize();
            Parser parser(tokens);
            ASTNode* root = parser.parse();

            long double res = root->evaluate(vars);

            // Show result but keep user text?
            // Let's replace text with result for standard calc behavior
            display->setText(QString::number((double)res));

            scene->clear();
            drawTreeSmart(root, 0, 0);
            view->centerOn(0, 0);
            delete root;
        } catch (const std::exception &e) {
             display->setText("Error");
        }
    }

    void solveEquation(std::string text) {
        try {
            size_t eqPos = text.find('=');
            std::string lhsStr = text.substr(0, eqPos);
            std::string rhsStr = text.substr(eqPos + 1);

            Parser p1(Lexer(lhsStr).tokenize());
            ASTNode* leftTree = p1.parse();

            Parser p2(Lexer(rhsStr).tokenize());
            ASTNode* rightTree = p2.parse();

            long double x = 1.0;
            long double h = 0.0001;
            bool found = false;

            for (int i = 0; i < 500; i++) { // Increased iterations
                vars["x"] = x;
                long double y = leftTree->evaluate(vars) - rightTree->evaluate(vars);

                if (std::abs(y) < 0.0001) { found = true; break; }

                vars["x"] = x + h;
                long double y_plus = leftTree->evaluate(vars) - rightTree->evaluate(vars);
                long double slope = (y_plus - y) / h;

                if (std::abs(slope) < 1e-9) slope = 1e-5; // Prevent div by zero
                x = x - (y / slope);
            }

            if (found) {
                display->setText("x = " + QString::number((double)x));
                scene->clear();
                drawTreeSmart(leftTree, 0, 0);
                view->centerOn(0, 0);
            } else {
                display->setText("No Solution");
            }
            delete leftTree; delete rightTree;
        } catch (const std::exception &e) {
            display->setText("Error");
        }
    }

    int getLeafCount(ASTNode* node) {
        if (!node) return 0;
        auto children = node->getChildren();
        if (children.empty()) return 1;
        int count = 0;
        for (auto child : children) count += getLeafCount(child);
        return count;
    }

    void drawTreeSmart(ASTNode* node, double x, double y) {
        if (!node) return;
        int r = 60; // Slightly smaller nodes for cleaner look
        int vGap = 120;
        int leafWidth = 80;

        QGraphicsEllipseItem *circle = scene->addEllipse(x, y, r, r, QPen(Qt::white, 3), QBrush(QColor("#00E5FF")));
        circle->setZValue(10);

        QGraphicsTextItem *text = scene->addText(QString::fromStdString(node->getLabel()));
        text->setDefaultTextColor(Qt::black);
        QFont font("Arial", 12, QFont::Bold);
        text->setFont(font);

        QRectF textRect = text->boundingRect();
        text->setPos(x + (r - textRect.width())/2, y + (r - textRect.height())/2);
        text->setZValue(11);

        auto children = node->getChildren();
        if (children.empty()) return;

        double totalWidth = 0;
        std::vector<double> childWidths;
        for (auto child : children) {
            double w = getLeafCount(child) * leafWidth;
            childWidths.push_back(w);
            totalWidth += w;
        }

        double currentChildX = x - (totalWidth / 2.0);
        for (size_t i = 0; i < children.size(); ++i) {
            double childRealX = currentChildX + (childWidths[i] / 2.0);
            QGraphicsLineItem *line = scene->addLine(x + r/2, y + r, childRealX + r/2, y + vGap, QPen(QColor("#AAAAAA"), 2));
            line->setZValue(0);
            drawTreeSmart(children[i], childRealX, y + vGap);
            currentChildX += childWidths[i];
        }
    }
};
#endif