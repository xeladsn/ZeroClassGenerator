#include<fencodegenere.h>

FenCodeGenere::FenCodeGenere(QString &codeHeader, QString &codeCpp, QWidget *parent = 0) : QTabWidget(parent)
{
    // Création des QTextEdit
    codeGenereHeaderTextEdit = new QTextEdit();
    codeGenereHeaderTextEdit->setPlainText(codeHeader);
    codeGenereHeaderTextEdit->setReadOnly(true);
    codeGenereHeaderTextEdit->setFont(QFont("Courier"));
    codeGenereHeaderTextEdit->setLineWrapMode(QTextEdit::NoWrap);

    codeGenereCppTextEdit = new QTextEdit();
    codeGenereCppTextEdit->setPlainText(codeCpp);
    codeGenereCppTextEdit->setReadOnly(true);
    codeGenereCppTextEdit->setFont(QFont("Courier"));
    codeGenereCppTextEdit->setLineWrapMode(QTextEdit::NoWrap);


    // Création des QDialog contenant les QTextEdit
    codeHeaderDialog = new QDialog;
    codeCppDialog = new QDialog;

    // Boutons "fermer" pour les 2 onglets
    fermerHeaderBtn = new QPushButton("Fermer");
    fermerCppBtn = new QPushButton("Fermer");

    // Boutons "enregistrer" pour les 2 onglets
    saveHeaderBtn = new QPushButton("Enregistrer sous");
    saveCppBtn = new QPushButton("Enregistrer sous");


    // Création des layouts
    // pour les boutons "enregistrer" et "fermer"
    QHBoxLayout *boutonsHeaderLayout = new QHBoxLayout;
    boutonsHeaderLayout->addWidget(saveHeaderBtn);
    boutonsHeaderLayout->addWidget(fermerHeaderBtn);
    QHBoxLayout *boutonsCppLayout = new QHBoxLayout;
    boutonsCppLayout->addWidget(saveCppBtn);
    boutonsCppLayout->addWidget(fermerCppBtn);
    // pour les QDialog
    QVBoxLayout *codeHeaderDialogLayout = new QVBoxLayout;
    QVBoxLayout *codeCppDialogLayout = new QVBoxLayout;
    codeHeaderDialogLayout->addWidget(codeGenereHeaderTextEdit);
    codeCppDialogLayout->addWidget(codeGenereCppTextEdit);
    codeHeaderDialogLayout->addLayout(boutonsHeaderLayout);
    codeCppDialogLayout->addLayout(boutonsCppLayout);
    codeHeaderDialog->setLayout(codeHeaderDialogLayout);
    codeCppDialog->setLayout(codeCppDialogLayout);

    // Création des onglets contenant les QDialog
    addTab(codeHeaderDialog, QString("&header"));
    addTab(codeCppDialog, QString("&cpp"));

    resize(350, 450);
    // connexions entre signaux et slots
    connect(fermerHeaderBtn, SIGNAL(clicked()), this, SLOT(close()));
    connect(fermerCppBtn, SIGNAL(clicked()), this, SLOT(close()));
    connect(saveHeaderBtn, SIGNAL(clicked()), this, SLOT(saveFileHeader()));
}


// Implémentation des slots personnalisés

void FenCodeGenere::saveFileHeader()
// sauvegarde le texte du header généré dans un fichier .txt avec un emplacement
// et un nom choisi par l'utilisateur
{
    // demande à l'utilisateur le chemin et le nom du fichier
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save file"), "header.txt", tr(".txt"));
    // création du fichier texte et enregistrement dans le bon répertoire
    QFile file(fileName);
    if (file.open(QIODevice::ReadWrite))
    {
        QTextStream stream(&file);
        QString textHeader = codeGenereHeaderTextEdit->toPlainText();
        stream << textHeader << endl;
    }
}
