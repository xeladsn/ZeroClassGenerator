#include <fenprincipale.h>
#include <fencodegenere.h>


FenPrincipale::FenPrincipale()
{
// Création des layouts et des widgets


    // Groupe : Définition de la classe
    nom = new QLineEdit;
    classeMere = new QLineEdit;
    attributs = new QListWidget;
    QLabel *labelAttributs = new QLabel("Attributs");
    ajouterAttributBtn = new QPushButton("&Ajouter");
    supprimerAttributBtn = new QPushButton("&Supprimer");
    QHBoxLayout *attributsLayout = new QHBoxLayout;
    attributsLayout->addWidget(labelAttributs);
    attributsLayout->addWidget(attributs);
    QHBoxLayout *boutonsAttributsLayout = new QHBoxLayout;
    boutonsAttributsLayout->addWidget(ajouterAttributBtn);
    boutonsAttributsLayout->addWidget(supprimerAttributBtn);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("&Nom", nom);
    formLayout->addRow("Classe &mère", classeMere);
    formLayout->addItem(attributsLayout);
    formLayout->addItem(boutonsAttributsLayout);

    QGroupBox *groupDefinition = new QGroupBox("Définition de la classe");
    groupDefinition->setLayout(formLayout);


    // Groupe : options
    protections = new QCheckBox("Protéger le header contre les inclusions multiples");
    protections->setChecked(true);
    headerGuard = new QLineEdit; // headerGuard sert à afficher en temps réel le header guard
    genererConstructeur = new QCheckBox("Générer un &constructeur par défaut");
    genererDestructeur = new QCheckBox("Générer un &destructeur");

    QHBoxLayout *headerLayout = new QHBoxLayout; // layout contenant les widgets relatifs au header guard
    headerLayout->addWidget(protections);
    headerLayout->addWidget(headerGuard);

    QVBoxLayout *optionsLayout = new QVBoxLayout;

    optionsLayout->addLayout(headerLayout);
    optionsLayout->addWidget(genererConstructeur);
    optionsLayout->addWidget(genererDestructeur);

    QGroupBox *groupOptions = new QGroupBox;
    groupOptions->setLayout(optionsLayout);


    // Groupe : commentaires
    auteur = new QLineEdit;
    date = new QDateEdit;
    date->setDate(QDate::currentDate());
    role = new QTextEdit;

    QFormLayout *commentairesLayout = new QFormLayout;
    commentairesLayout->addRow("&Auteur", auteur);
    commentairesLayout->addRow("Da&te de création", date);
    commentairesLayout->addRow("&Role de la classe", role);

    groupCommentaires = new QGroupBox("Ajouter des commentaires");
    groupCommentaires->setCheckable(true);
    groupCommentaires->setChecked(false);
    groupCommentaires->setLayout(commentairesLayout);

    // Groupe : boutons du bas (générer, quitter)
    generer = new QPushButton("&Générer !");
    quitter = new QPushButton("&Quitter");

    QHBoxLayout *boutonsLayout = new QHBoxLayout;
    boutonsLayout->setAlignment(Qt::AlignRight);

    boutonsLayout->addWidget(generer);
    boutonsLayout->addWidget(quitter);

    // Définition du layout principal, du titre de la fenêtre, etc.
    QVBoxLayout *layoutPrincipal = new QVBoxLayout;
    layoutPrincipal->addWidget(groupDefinition);
    layoutPrincipal->addWidget(groupOptions);
    layoutPrincipal->addWidget(groupCommentaires);
    layoutPrincipal->addLayout(boutonsLayout);

    setLayout(layoutPrincipal);
    setWindowTitle("Zero Class Generator");
    setWindowIcon(QIcon("icone.png"));
    resize(600, 600);


// Connexions des signaux et des slots
    connect(quitter, SIGNAL(clicked()), qApp, SLOT(quit())); // bouton quitter de la fenêtre principale
    connect(protections, SIGNAL(toggled(bool)), headerGuard, SLOT(setEnabled(bool))); // connexion entre la QCheckBox de protections et le QLineEdit headerGuard
    connect(nom, SIGNAL(textChanged(QString)), this, SLOT(ecrireHeaderGuard(QString))); // slot personnalisé pour afficher le bon header guard
    connect(generer, SIGNAL(clicked()), this, SLOT(genererCode())); // bouton générer pour afficher le code généré
    connect(ajouterAttributBtn, SIGNAL(clicked()), this, SLOT(addAttributInList())); // ajouter un attribut
    connect(supprimerAttributBtn, SIGNAL(clicked()), this, SLOT(deleteAttributInList())); // supprimer un attribut
}

void FenPrincipale::ecrireHeaderGuard(QString nom)
{
    headerGuard->setText(QString("HEADER_" + nom.toUpper()));
}

void FenPrincipale::addAttributInList()
{
    QString attributText = QInputDialog::getText(this, tr("Insert attribut"),
           tr("Input text for the new attribut:"));
    attributs->addItem(attributText);
}

void FenPrincipale::deleteAttributInList()
{
    attributs->takeItem(attributs->currentRow());
}

QString FenPrincipale::genererCodeHeader()
// génère une QString contenant le code du .h
{
    // On vérifie que le nom de la classe n'est pas vide, sinon on arrête
    if (nom->text().isEmpty())
    {
        QMessageBox::critical(this, "Erreur", "Veuillez entrer au moins un nom de classe");
        return "";
    }

    // Si tout va bien, on génère le code
    QString code;

    // Génération du code à l'aide des informations rentrées par l'utilisateur
    // bloc des commentaires
    if (groupCommentaires->isChecked())
    {
        code.append("/*\n");
        code.append("Auteur : " + auteur->text()+"\n");
        code.append("Date de création : " + date->date().toString() + "\n");
        code.append("Rôle :\n" + role->toPlainText() + "\n");
        code.append("*/\n\n");
    }
    // bloc des protections contre les inclusions multiples
    if (protections->isChecked())
    {

        code.append("#ifndef" + headerGuard->text() +"\n");
        code.append("#define" + headerGuard->text() +"\n");
        code.append("\n");
    }
    // bloc relatif à la classe
    code.append("class " + nom->text());
    if (!classeMere->text().isEmpty())
    {
        code.append(" : public " + classeMere->text() + "\n");
    }

    QString texteConstructeur = QString("");
    QString texteDestructeur = QString("");
    if (genererConstructeur->isChecked())
    {
        texteConstructeur = QString(nom->text()+"();");
    }

    if (genererDestructeur->isChecked())
    {
        texteDestructeur = QString("~"+nom->text()+"();");
    }

    code.append("{\n   public: \n      " + texteConstructeur + "\n");
    code.append("      " + texteDestructeur+"\n");
    code.append("\n");
    code.append("   protected:\n");
    code.append("\n");
    code.append("   private:\n");
    code.append("      ");
    // Ajoute des attributs de la liste dans le code généré
    for (int compteur(0) ; compteur < attributs->count() ; compteur++)
        {
            code.append(attributs->item(compteur)->text()+";\n");
            code.append("      ");
        }
    code.append("\n\n");

    if (protections->isChecked())
    {
        code.append("#endif\n");
    }

    return code;
}

QString FenPrincipale::genererCodeCpp()
// génère une QString contenant le code du .cpp
{
    QString code;
    // Génération du code à l'aide des informations rentrées par l'utilisateur
    // bloc des commentaires
    if (groupCommentaires->isChecked())
    {
        code.append("/*\n");
        code.append("Auteur : " + auteur->text()+"\n");
        code.append("Date de création : " + date->date().toString() + "\n");
        code.append("Rôle :\n" + role->toPlainText() + "\n");
        code.append("*/\n\n");
    }
    // bloc relatif à la classe
    code.append("#include<"+nom->text()+".h>\n\n");
    code.append("class" + nom->text() + ": public " + classeMere->text() + "\n{\n    ");
    code.append("public:\n        ");
    code.append(nom->text()+"();\n    ");
    code.append("private:\n");
    for (int compteur(0) ; compteur < attributs->count() ; compteur++)
    {
        code.append(attributs->item(compteur)->text()+";\n        ");
    }
    code.append("};");

    return code;
}

void FenPrincipale::genererCode()
{

    // On appelle la fonction génèrant le code du header
    QString codeHeader = genererCodeHeader();
    QString codeCpp = genererCodeCpp();
    // On crée puis affiche la fenêtre qui affichera le code généré, qu'on lui envoie en paramètre    
    FenCodeGenere *fenetreCode = new FenCodeGenere(codeHeader, codeCpp, this);
    fenetreCode->show();
}