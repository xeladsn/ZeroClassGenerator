#ifndef FENPRINCIPALE_H
#define FENPRINCIPALE_H

#include<QtWidgets>

class FenPrincipale : public QWidget
{
    Q_OBJECT  // indispensable lors de la création de slots

public:
    FenPrincipale();

public slots:
    void ecrireHeaderGuard(QString nom);
    void addAttributInList();
    void deleteAttributInList();

private slots:
    QString genererCodeHeader();
    QString genererCodeCpp();
    void genererCode();

protected:

private:
    QLineEdit *nom;
    QLineEdit *classeMere;
    QListWidget *attributs;
    QPushButton *ajouterAttributBtn;
    QPushButton *supprimerAttributBtn;
    QCheckBox *protections;
    QLineEdit *headerGuard;
    QCheckBox *genererConstructeur;
    QCheckBox *genererDestructeur;
    QGroupBox *groupCommentaires;
    QLineEdit *auteur;
    QDateEdit *date;
    QTextEdit *role;
    QPushButton *generer;
    QPushButton *quitter;

};

#endif // FENPRINCIPALE_H
