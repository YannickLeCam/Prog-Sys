#!/bin/sh

PROG="./custom_find"
TMP="/tmp/$$"


lightgreen='\033[1;32m'
neutral='\033[0m'
blue='\033[1;34m'
red='\033[1;31m'

print_error()
{
    echo "[${red}ECHEC${neutral}] : ${red}$1${neutral}"
}

print_ok()
{
    echo "${lightgreen}SUCCES${neutral}"
}

print_closetook()
{
    echo "[${blue}PETIT PROBLEME${neutral}] : ${blue}$1${neutral}"
}


check_empty ()
{
    if [ -s $1 ]; then
        return 0;
    fi

    return 1
}

# teste si le programme a échoué
# - code de retour du programme doit être égal à 1
# - stdout doit être vide
# - stderr doit contenir un message d'erreur
check_echec()
{
    if [ $1 -ne 1 ]; then
        print_error "échec => code de retour == $1 alors que 1 attendu"
        return 0
    fi

    if check_empty $TMP/stdout; then
        print_closetook "échec => sortie standard non vide"
        return 0
    fi

    if ! check_empty $TMP/stderr; then
        print_closetook "échec => sortie erreur vide"
        return 0
    fi

    return 1
}


# Teste si le répertoire passé en argument existe
# - code de retoour = 2
# - stdout doit être vide
# - stderr doit afficher un message
check_bad_args()
{
    if [ $1 -ne 2 ]; then
        print_error "échec => code de retour == $1 alors que 2 attendu"
        return 0
    fi

    if check_empty $TMP/stdout; then
        print_closetook "échec => sortie standard non vide mais code de retour correct"
        return 0
    fi

    if ! check_empty $TMP/stderr; then
        print_closetook "échec => sortie erreur vide mais code de retour correct"
        return 0
    fi

    return 1
}


# teste si le pg a réussi
# - code de retour du pg doit être égal à 0
# - stdout et stderr doivent être vides
check_success()
{
    if [ $1 -ne 0 ]; then
        print_error "échec => code de retour == $1 alors que 0 attendu"
        return 0
    fi

    if ! check_empty $TMP/stdout; then
       print_closetook "échec => sortie standard non vide mais code de retour correct"
       return 0
    fi

    if check_empty $TMP/stderr; then
        print_closetook "échec => sortie erreur non vide mais code de retour correct"
        return 0
    fi

    return 1
}


test_1()
{
    echo ${lightgreen} "\n############## Test 1 - Mémoire ##############\n" ${neutral}
    mkdir $TMP/test
    touch $TMP/test/coucou.txt
    $PROG $TMP/test wc -l > $TMP/stdout 2> $TMP/stderr
    res=$?
    rm -rf $TMP/test
    if [ ${res} != 0 ] || check_empty $TMP/stderr;
    then
        print_error "Problème au niveau de la gestion des buffer, veuillez régler ce \
        soucis avant de passer à la suite des tests\nPour vérifier ceci, compilez avec \ 
        le makefile et lancez votre programme. Si ce test ne passe pas, il est probable \
        que les autres tests ne passent pas, même si votre programme est correct."
        return 1
    fi
    print_ok

    return 0
}


test_2()
{
    echo ${lightgreen} "############## Test 2 - arguments ##############\n" ${neutral}

    echo "-- Test a) - sans argument"
    $PROG                          > $TMP/stdout 2> $TMP/stderr
    if check_bad_args $?;                             then return 1; fi
    print_ok

    echo  "-- Test b) - 1 argument"
    $PROG /bin                  > $TMP/stdout 2> $TMP/stderr
    if check_bad_args $?;                             then return 1; fi
    print_ok

    echo  "-- Test c) - Arguments OK"
    mkdir $TMP/test
    $PROG $TMP/test sha1sum            > $TMP/stdout 2> $TMP/stderr
    res=$?
    rmdir $TMP/test
    if [ ${res} != 0 ];                            
    then
        print_error "Le test a échoué alors qu'il aurait du fonctionner"
        return 1
    fi
    print_ok

    echo  "-- Test d) - répertoire d'entrée inexistant"
    $PROG coucou_dir sha1sum           > $TMP/stdout 2> $TMP/stderr
    if check_echec $?;                             then return 1; fi
    print_ok

    return 0
}


test_3 ()
{
    echo ${lightgreen} "\n############## Test 3 - Test des erreurs ##############\n" ${neutral}

    echo "-- Test a) test sur entrée qui n'est ni fichier ni répertoire"
    mkdir $TMP/test
    ln -s /bin/cat $TMP/test/test_link
    $PROG $TMP/test wc -l > $TMP/stdout 2> $TMP/stderr
    res=$?
    unlink $TMP/test/test_link
    rmdir $TMP/test

    if [ ${res} != 0 ] || ! check_empty $TMP/stderr;
    then
        print_error "Mauvaise gestion des entrées autres que fichier ou répertoires"
        return 1
    fi

    print_ok

    echo  "-- Test b) commande inexistante"

    $PROG /usr/bin cinjzfv     > $TMP/stdout 2> $TMP/stderr
    if check_echec $?;                             then return 1; fi
    print_ok

    echo "-- Test c) test sur un répertoire impossible à lire"
    mkdir $TMP/test
    chmod -r $TMP/test
    $PROG $TMP/test wc -l > $TMP/stdout 2> $TMP/stderr
    res=$?
    chmod +r $TMP/test
    rmdir $TMP/test

    if check_echec ${res};                             then return 1; fi
    print_ok

    echo "-- Test c) test sur un répertoire impossible à lire - bis"
    mkdir $TMP/test
    touch $TMP/test/coucou.txt
    chmod -x $TMP/test
    $PROG $$ wc -l > $TMP/stdout 2> $TMP/stderr
    res=$?
    chmod +x $TMP/test
    rm -R $TMP/test

    if check_echec ${res};                             then return 1; fi
    print_ok

    return 0
}



test_4 ()
{
    echo ${lightgreen} "\n############## Test 4 - Fonctionnement global ##############\n" ${neutral}

    echo "-- Test a) test sur répertoire vide"
    mkdir $TMP/test
    $PROG $TMP/test sha1sum    > $TMP/stdout 2> $TMP/stderr
    rmdir $TMP/test
    if check_empty $TMP/stdout;        then return 1; fi
    print_ok

    echo  "-- Test b) avec une commande simple"
    $PROG /usr/bin sha1sum 2> $TMP/stderr | sed -e 's/^[ \t]*//' | sort   > $TMP/out1
    find /usr/bin -type f 2> $TMP/stderr | xargs sha1sum | sed -e 's/^[ \t]*//' | sort > $TMP/out2
    
    if ! cmp $TMP/out1 $TMP/out2 ;        
    then 
        print_error "Output incorrect, ne correspond pas à l'output de find"
        return 1
    fi
    print_ok

    echo  "-- Test c) avec une commande avec arguments"
    $PROG /usr/bin stat -c %n:%s 2> $TMP/stderr | sed -e 's/^[ \t]*//' | sort   > $TMP/out1 
    find /usr/bin -type f | xargs stat -c %n:%s 2> $TMP/stderr | sed -e 's/^[ \t]*//' | sort > $TMP/out2
    
    if ! cmp $TMP/out1 $TMP/out2 ;        
    then 
        print_error "Output incorrect, ne correspond pas à l'output de find"
        return 1    
    fi
    print_ok

    echo  "-- Test d) test sur arborescence aléatoire"
    ./test_arbo.sh $TMP/rndpath > /dev/null
    $PROG $TMP/rndpath sha1sum | sed -e 's/^[ \t]*//' | sort  > $TMP/out1
    find $TMP/rndpath -type f | xargs sha1sum | sed -e 's/^[ \t]*//' | sort > $TMP/out2

    rm -r $TMP/rndpath
    
    if ! cmp $TMP/out1 $TMP/out2 ;        
    then 
        print_error "Output incorrect, ne correspond pas à l'output de find"
        return 1
    fi
    print_ok

    return 0
}



mkdir $TMP

nb_test=0

for T in $(seq 1 4)
do
	if test_$T; then
		echo ${lightgreen} "== Test $T : ok $T/4\n" ${neutral}
        nb_test=$((nb_test+1))
	else
		echo ${red} "== Test $T : échec" ${neutral}
	fi
done


if [ $nb_test -eq 4 ];
then
    echo "Vos tests passent tous, rendez votre code propre afin d'avoir une excellente note !"
fi

rm -R $TMP
