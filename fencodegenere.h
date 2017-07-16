#ifndef FENCODEGENERE_H
#define FENCODEGENERE_H

#include<QtWidgets>

class FenCodeGenere : public QTabWidget
{
    Q_OBJECT

public:
    FenCodeGenere(QString &codeHeader, QString &codeCpp, QWidget *parent);

public slots:
    //void saveFileCpp();
    void saveFileHeader();

private:
    QDialog *codeHeaderDialog;
    QDialog *codeCppDialog;
    QPushButton *saveHeaderBtn;
    QPushButton *saveCppBtn;
    QTextEdit *codeGenereHeaderTextEdit;
    QTextEdit *codeGenereCppTextEdit;
    QPushButton *fermerHeaderBtn;
    QPushButton *fermerCppBtn;
};

#endif // FENCODEGENERE_H
