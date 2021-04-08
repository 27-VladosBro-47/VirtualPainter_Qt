#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setFixedSize(350, 367);

    colorDetector = new ColorDetector();
    painter = new Painter();

    connect(this, &MainWindow::setProcessingStatusPainter, painter, &Painter::setProcessingStatus);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setDefaultSettings()
{
    ui->horizontalSlider->setRange(0, 179);
    ui->horizontalSlider_2->setRange(0, 179);
    ui->horizontalSlider_3->setRange(0, 255);
    ui->horizontalSlider_4->setRange(0, 255);
    ui->horizontalSlider_5->setRange(0, 255);
    ui->horizontalSlider_6->setRange(0, 255);

    ui->horizontalSlider->setSliderPosition(0);
    ui->horizontalSlider_2->setSliderPosition(179);
    ui->horizontalSlider_3->setSliderPosition(0);
    ui->horizontalSlider_4->setSliderPosition(255);
    ui->horizontalSlider_5->setSliderPosition(0);
    ui->horizontalSlider_6->setSliderPosition(255);

    ui->label_7->setText(QString::number(ui->horizontalSlider->value()));
    ui->label_8->setText(QString::number(ui->horizontalSlider_2->value()));
    ui->label_9->setText(QString::number(ui->horizontalSlider_3->value()));
    ui->label_10->setText(QString::number(ui->horizontalSlider_4->value()));
    ui->label_11->setText(QString::number(ui->horizontalSlider_5->value()));
    ui->label_12->setText(QString::number(ui->horizontalSlider_6->value()));

    ui->label_14->setStyleSheet("background-color: rgb(255, 255, 255);");

    ui->pushButton_7->setEnabled(0);
    ui->pushButton_3->setEnabled(1);
}

// Button "Start Virtual Painter"
void MainWindow::on_pushButton_clicked()
{
    if(colorDetector->isVectorsEmpty())
    {
        ui->stackedWidget->setCurrentIndex(2);

        painter->setColorVectors(colorDetector->getColors(), colorDetector->getBrushColors());

        emit setProcessingStatusPainter(true);
        std::thread th1([&]()
        {
            painter->StartProcessing();
        });
        th1.detach();
    }
    else
    {
       QMessageBox::about(this, "Wait", "Sorry\nFirst of all, you must add some colors");
    }
}

// Button "Add colors"
void MainWindow::on_pushButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

    delete colorDetector;
    colorDetector = nullptr;

    colorDetector = new ColorDetector();

    setDefaultSettings();

    connect(this, &MainWindow::setProcessingStatusColorDetector, colorDetector, &ColorDetector::setProcessingStatus);
    emit setProcessingStatusColorDetector(true);
    std::thread th1([&]()
    {
        colorDetector->StartProcessing();
    });

    th1.detach();

}

// Button "Back to the menu" ("Add colors" window)
void MainWindow::on_pushButton_5_clicked()
{
    emit setProcessingStatusColorDetector(false);
    ui->stackedWidget->setCurrentIndex(0);
}

// Button "Back to the menu" ("Start Virtual Painter" window)
void MainWindow::on_pushButton_6_clicked()
{
    emit setProcessingStatusPainter(false);
    ui->stackedWidget->setCurrentIndex(0);
}

// Hue min
void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->label_7->setText(QString::number(value));

    colorDetector->getRefHsvMin(0) = value;
}

// Hue max
void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    ui->label_8->setText(QString::number(value));

    colorDetector->getRefHsvMax(0) = value;
}

// Saturation min
void MainWindow::on_horizontalSlider_3_valueChanged(int value)
{
    ui->label_9->setText(QString::number(value));

    colorDetector->getRefHsvMin(1) = value;
}

// Saturation max
void MainWindow::on_horizontalSlider_4_valueChanged(int value)
{
    ui->label_10->setText(QString::number(value));

    colorDetector->getRefHsvMax(1) = value;
}

// Value min
void MainWindow::on_horizontalSlider_5_valueChanged(int value)
{
    ui->label_11->setText(QString::number(value));

    colorDetector->getRefHsvMin(2) = value;
}

// Value max
void MainWindow::on_horizontalSlider_6_valueChanged(int value)
{
    ui->label_12->setText(QString::number(value));

    colorDetector->getRefHsvMax(2) = value;
}

// Button "Add default buttons"
void MainWindow::on_pushButton_3_clicked()
{
    // Yellow
    colorDetector->addColors(32, 40, 146, 73, 148, 255);
    colorDetector->addBrushColors(0, 225, 225);

    // Green
    colorDetector->addColors(150, 70, 152, 179, 168, 255);
    colorDetector->addBrushColors(101, 65, 225);

    // Pink
    colorDetector->addColors(72, 135, 135, 96, 255, 255);
    colorDetector->addBrushColors(101, 206, 68);

    QMessageBox::about(this, "Add default colors", "Colors have been added");

    ui->pushButton_3->setEnabled(0);
}

// Button "Set brush color"
void MainWindow::on_pushButton_4_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this,"Brush color");
    if(color.isValid())
    {
        QString strColor = "(" + QString::number(color.red()) + ", " +
                                 QString::number(color.green()) + ", " +
                                 QString::number(color.blue()) + ");";

        ui->label_14->setStyleSheet("background-color: rgb" + strColor);

        colorDetector->setBRG(color.blue(), color.green(), color.red());

        ui->pushButton_7->setEnabled(1);
    }
}

// Button "Add Color"
void MainWindow::on_pushButton_7_clicked()
{
    colorDetector->addColors();
    colorDetector->addBrushColors();

    QMessageBox::about(this, "Add Color", "Color has been added");

    ui->pushButton_7->setEnabled(0);
}
