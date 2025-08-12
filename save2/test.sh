#!/bin/bash
# test_minishell.sh

# Couleurs pour l'affichage
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Chemin vers votre minishell
MINISHELL_PATH="./minishell"

# Fonction pour afficher les titres
print_header() {
    echo -e "\n${BLUE}=== $1 ===${NC}"
}

# Fonction pour tester une commande
test_command() {
    local cmd="$1"
    local test_name="$2"
    
    echo -e "\n${YELLOW}Test: $test_name${NC}"
    echo -e "${BLUE}Commande:${NC} $cmd"
    
    # Test avec bash
    echo -e "${GREEN}Bash résultat:${NC}"
    bash -c "$cmd" 2>&1
    local bash_exit=$?
    
    # Test avec minishell (si disponible)
    if [ -f "$MINISHELL_PATH" ]; then
        echo -e "${GREEN}Minishell résultat:${NC}"
        echo "$cmd" | timeout 5s "$MINISHELL_PATH" 2>&1
        local mini_exit=$?
        
        if [ $mini_exit -eq 124 ]; then
            echo -e "${RED}TIMEOUT - Minishell n'a pas répondu${NC}"
        fi
    else
        echo -e "${RED}Minishell non trouvé à: $MINISHELL_PATH${NC}"
    fi
    
    echo "----------------------------------------"
}

# Fonction pour tester en mode interactif
interactive_test() {
    local cmd="$1"
    local test_name="$2"
    
    echo -e "\n${YELLOW}Test: $test_name${NC}"
    echo -e "${BLUE}Commande:${NC} $cmd"
    
    # Test avec bash
    echo -e "${GREEN}Bash résultat:${NC}"
    bash -c "$cmd" 2>&1
    
    # Demander le résultat de minishell
    echo -e "${GREEN}Testez maintenant dans votre minishell et entrez le résultat:${NC}"
    echo -e "${BLUE}À taper dans minishell:${NC} $cmd"
    read -p "Résultat minishell: " minishell_result
    echo -e "${GREEN}Vous avez saisi:${NC} $minishell_result"
    echo "----------------------------------------"
}

# Menu principal
echo -e "${BLUE}Script de test Minishell vs Bash${NC}"
echo "1. Test automatique (nécessite ./minishell)"
echo "2. Test interactif (vous tapez les résultats)"
echo "3. Générer fichier de commandes seulement"
read -p "Choisissez une option (1-3): " choice

case $choice in
    1)
        TEST_MODE="auto"
        ;;
    2)
        TEST_MODE="interactive"
        ;;
    3)
        TEST_MODE="generate"
        ;;
    *)
        echo "Option invalide"
        exit 1
        ;;
esac

# Array des tests
declare -a tests=(
    # [commande]|[nom du test]
    'echo "Hello World"|Guillemets doubles simples'
    'echo '\''Hello World'\''|Guillemets simples'
    'echo $USER|Variable simple'
    'echo "$USER"|Variable dans guillemets doubles'
    'echo '\''$USER'\''|Variable dans guillemets simples'
    'echo "Hello $USER"|Variable avec texte'
    'echo '\''Hello $USER'\''|Variable avec texte (guillemets simples)'
    'echo \$USER|Échappement de variable'
    'echo "Hello '\''$USER'\'' world"|Guillemets simples dans doubles'
    'echo '\''Hello "$USER" world'\''|Guillemets doubles dans simples'
    'echo "I said \"Hello $USER\""|Échappement de guillemets'
    'echo \$USER is $USER|Mélange échappé et non échappé'
    'echo \\$USER|Double backslash'
    'echo "\\\$USER"|Triple backslash dans guillemets'
    'echo "Hello"'\''World'\''|Concaténation de guillemets'
    'echo "$USER'\''s home"|Apostrophe dans expansion'
    'echo "$$"|Variable PID'
    'echo '\''$$'\''|Variable PID (guillemets simples)'
    'echo $NOTEXIST|Variable inexistante'
    'echo "$NOTEXIST"|Variable inexistante (guillemets)'
    'echo ""|Chaîne vide (guillemets doubles)'
    'echo '\'''\''|Chaîne vide (guillemets simples)'
    'echo Hello\ \ \ World|Espaces échappés'
    'echo "   espaces   "|Espaces dans guillemets'
)

if [ "$TEST_MODE" = "generate" ]; then
    # Générer fichier de commandes
    echo "# Commandes de test pour minishell" > test_commands.txt
    for test in "${tests[@]}"; do
        cmd="${test%|*}"
        echo "$cmd" >> test_commands.txt
    done
    echo "Fichier test_commands.txt généré avec ${#tests[@]} commandes."
    echo "Usage: bash < test_commands.txt > bash_results.txt"
    echo "       ./minishell < test_commands.txt > minishell_results.txt"
    echo "       diff bash_results.txt minishell_results.txt"
    exit 0
fi

# Exécuter les tests
print_header "Tests de comparaison Bash vs Minishell"

for test in "${tests[@]}"; do
    cmd="${test%|*}"
    name="${test#*|}"
    
    if [ "$TEST_MODE" = "auto" ]; then
        test_command "$cmd" "$name"
    else
        interactive_test "$cmd" "$name"
    fi
done

print_header "Tests d'erreurs"

# Tests d'erreurs
error_tests=(
    'echo "non fermé|Guillemets non fermés'
    'echo '\''non fermé|Guillemets simples non fermés'
)

for test in "${error_tests[@]}"; do
    cmd="${test%|*}"
    name="${test#*|}"
    
    if [ "$TEST_MODE" = "auto" ]; then
        test_command "$cmd" "$name"
    else
        interactive_test "$cmd" "$name"
    fi
done

echo -e "\n${GREEN}Tests terminés !${NC}"

if [ "$TEST_MODE" = "auto" ]; then
    echo -e "${YELLOW}Note:${NC} Comparez visuellement les résultats."
    echo -e "Pour des tests plus précis, utilisez le mode génération avec diff."
fi