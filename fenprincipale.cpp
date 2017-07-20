#include <fenprincipale.h>
#include <fencodegenere.h>


FenPrincipale::FenPrincipale()
// Constructeur de la fenetre principale
{
// Création des layouts et des widgets


    // Groupe : Définition de la classe
    // initialisation des attributs
    nom = new QLineEdit;
    classeMere = new QLineEdit;
    attributs = new QListWidget;

    // contrôle de l'input de l'utilisateur
    // max 20 characters for the input
    // and ASCII alphanumeric character required
    QString inputMaskClass(20, *QString("N").data());
    nom->setInputMask(inputMaskClass);
    classeMere->setInputMask(inputMaskClass);
    attributs->setInputMask(inputMaskClass);

    // suite de l'initialisation des attributs
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
    genererConstructeur = new QCheckBox("Générer un &constructeur par défaut.");
    genererDestructeur = new QCheckBox("Générer un &destructeur.");
    genererAccesseurs = new QCheckBox("Générer les &accesseurs.");

    QHBoxLayout *headerLayout = new QHBoxLayout; // layout contenant les widgets relatifs au header guard
    headerLayout->addWidget(protections);
    headerLayout->addWidget(headerGuard);

    QVBoxLayout *optionsLayout = new QVBoxLayout;

    optionsLayout->addLayout(headerLayout);
    optionsLayout->addWidget(genererConstructeur);
    optionsLayout->addWidget(genererDestructeur);
    optionsLayout->addWidget(genererAccesseurs);

    QGroupBox *groupOptions = new QGroupBox;
    groupOptions->setLayout(optionsLayout);


    // Groupe : commentaires
    auteur = new QLineEdit;
    date = new QDateEdit;
    date->setDate(QDate::currentDate());
    role = new QTextEdit;

    QFormLayout *commentairesLayout = new QFormLayout;
    commentairesLayout->addRow("&Auteur", auteur);
    commentairesLayout->addRow("&Date de création", date);
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
// slot personnalisé pour que l'utilisateur ajoute un attribut à la liste d'attributs
{
    QString attributText = QInputDialog::getText(this, tr("Nouvel attribut"),
           tr("Entrez ici votre nouvel attribut:"));
    QString attributType = QInputDialog::getText(this, tr("Type de l'attribut"),
                                                 tr("Entrez ici le type du nouvel attribut:"));
    attributs->addItem(attributType + " " + attributText);
}

void FenPrincipale::deleteAttributInList()
{
    attributs->takeItem(attributs->currentRow());
}

QString FenPrincipale::genererCodeHeader()
// génère une QString contenant le code du .h
{
    QString const indentation("    ");

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
    QString texteAccesseurs = QString("");
    if (genererConstructeur->isChecked())
    {
        texteConstructeur = QString(nom->text()+"();");
    }

    if (genererDestructeur->isChecked())
    {
        texteDestructeur = QString("~"+nom->text()+"();");
    }
    if (genererAccesseurs->isChecked())
    {
        // si la liste des attributs est vide on affiche une erreur
        if (attributs->count()==0)
        {
            QMessageBox::critical(this, "Erreur", "Veuillez entrer au moins un attribut.");
            return "";
        }
        // sinon
        for (int compteur(0) ; compteur < attributs->count() ; compteur++)
            {
                QString typeAttribut(attributs->item(compteur)->text().split(" ").at(0));
                QString nomAttribut(attributs->item(compteur)->text().split(" ").at(1));
                QStringRef nomAttribut2(&nomAttribut, 1, nomAttribut.count()-1);
                texteAccesseurs.append(indentation + typeAttribut + " " + "get"
                                       + nomAttribut.at(0).toUpper() + nomAttribut2 + "();\n");
            }

    }
    code.append("{\n");
    code.append("public:\n");
    code.append(indentation + texteConstructeur + "\n");
    code.append(indentation + texteDestructeur + "\n");
    code.append(texteAccesseurs);
    code.append("\n");

    code.append("protected:\n");
    code.append("\n");

    code.append("private:\n");
    // ajout des attributs de la liste d'attributs dans le code généré
    for (int compteur(0) ; compteur < attributs->count() ; compteur++)
        {
            QString typeAttribut(attributs->item(compteur)->text().split(" ").at(0));
            QString nomAttribut(attributs->item(compteur)->text().split(" ").at(1));
            code.append(indentation + typeAttribut + " " + "*" + nomAttribut + ";\n");
        }
    code.append("\n");
    code.append("};");
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
    QString const indentation("    ");

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
    // bloc d'inclusion du header
    code.append("#include<"+nom->text()+".h>\n\n");

    // bloc du constructeur
    code.append(nom->text() + "::" + nom->text() + " : public " + classeMere->text() + "\n{");
    code.append("\n");
    // ajout des attributs de la liste d'attributs dans le corps du constructeur
    for (int compteur(0) ; compteur < attributs->count() ; compteur++)
        {
            QString typeAttribut(attributs->item(compteur)->text().split(" ").at(0));
            QString nomAttribut(attributs->item(compteur)->text().split(" ").at(1));
            code.append(indentation + nomAttribut + " = " + "new " + typeAttribut + ";\n");
        }
    code.append("};");
    code.append("\n\n");

    // bloc des getter functions
    if (genererAccesseurs->isChecked())
    {
        QString texteAccesseursCpp(QString(""));
        for (int compteur(0) ; compteur < attributs->count() ; compteur++)
            {
                QString typeAttribut(attributs->item(compteur)->text().split(" ").at(0));
                QString nomAttribut(attributs->item(compteur)->text().split(" ").at(1));
                QStringRef nomAttribut2(&nomAttribut, 1, nomAttribut.count()-1);
                texteAccesseursCpp.append(typeAttribut + " " + nom->text() + "::" + "get"
                                       + nomAttribut.at(0).toUpper() + nomAttribut2 + "()\n");
                texteAccesseursCpp.append("{\n");
                texteAccesseursCpp.append(indentation + "return " + nomAttribut + ";");
                texteAccesseursCpp.append("\n}\n");
            }
        code.append(texteAccesseursCpp);
    }
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
