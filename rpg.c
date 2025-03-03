#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>  // Para rand(), srand()
#include <time.h>    // Para time()

// Constantes para el juego
#define MAX_VIDA 100
#define DANIO_ESPADA_MIN 1
#define DANIO_ESPADA_MAX 50
#define DANIO_LOBO_MIN 10
#define DANIO_LOBO_MAX 20
#define DANIO_OGRO_MIN 10
#define DANIO_OGRO_MAX 30
#define DANIO_DRAGON_MIN 25
#define DANIO_DRAGON_MAX 60
#define CURA_POCION 40
#define VIDA_LOBO 75
#define VIDA_OGRO 150
#define VIDA_DRAGON 450

// Función para inicializar la vida
void inicializarVida(int *vida, const char *nombre, int vidaMaxima) {
    *vida = vidaMaxima;
    printf("%s = %d\n", nombre, *vida);
}

// Función para mostrar un divisor visual
void mostrarDivisor() {
    printf("\n----------------------------------------\n");
}

// Función para mostrar el estado actual del jugador y el enemigo
void mostrarEstado(int hp, int ep, int pociones, const char *nombreEnemigo) {
    printf("\n--- Estado Actual ---\n");
    printf("Tu vida: %d | Vida del %s: %d | Pociones: %d\n", hp, nombreEnemigo, ep, pociones);
}

// Función para calcular el daño con rango y crítico
int calcularDanio(int min, int max, const char *atacante, const char *objetivo) {
    int danio = min + rand() % (max - min + 1); // Daño aleatorio entre min y max
    int max1 = 15, min1 = 1;
    int r = rand() % (max1 - min1 + 1);
    if (r == 13 || r == 14 || r == 15) {
        danio = danio * 2;
        printf("¡Golpe crítico de %s! 💥 %s recibe %d de daño.\n", atacante, objetivo, danio);
    } else {
        printf("%s ataca a %s y le hace %d de daño.\n", atacante, objetivo, danio);
    }
    return danio;
}

// Función para el ataque especial con la matadragones
int matadragones(int *hp, int *ep, int *pociones, int min, int max, const char *atacante, const char *objetivo) {
    int danio = min + rand() % (max - min + 1); // Daño aleatorio entre min y max
    printf("La matadragones brilla con poder... ¡Tu daño se duplica!\n");
    danio = danio * 2;
    int max1 = 15, min1 = 1;
    int r = rand() % (max1 - min1 + 1);
    if (r == 13 || r == 14 || r == 15) {
        danio = danio * 2;
        printf("¡Golpe crítico de %s! 💥 %s recibe %d de daño.\n", atacante, objetivo, danio);
    } else {
        printf("%s ataca a %s y le hace %d de daño.\n", atacante, objetivo, danio);
    }
    return danio;
}

// Función para el turno del jugador
void turnoJugador(int *hp, int *ep, int *pociones, bool *cargarMatadragones) {
    int opcion;

    // Si la matadragones fue activada en el turno anterior, atacar automáticamente
    if (*cargarMatadragones) {
        printf("¡Ataque automático con la matadragones!\n");
        int danio = matadragones(hp, ep, pociones, DANIO_ESPADA_MIN, DANIO_ESPADA_MAX, "Jugador", "Enemigo");
        *ep -= danio;
        *cargarMatadragones = false; // Reiniciar el estado
        return;
    }

    // Menú normal del jugador
    printf("\n¿Qué deseas hacer?\n");
    printf("1 - Espadazo 🗡️ (haces entre %d y %d de daño)\n", DANIO_ESPADA_MIN, DANIO_ESPADA_MAX);
    printf("2 - Usar poción 🍷 (cura %d de vida, tienes %d pociones)\n", CURA_POCION, *pociones);
    printf("3 - Usar la matadragones\n");
    printf("Elige una opción: ");
    while (scanf("%d", &opcion) != 1 || (opcion != 1 && opcion != 2 && opcion != 3)) {
        printf("❌ Opción inválida. Introduce 1, 2 o 3: ");
        while (getchar() != '\n'); // Limpiar el buffer de entrada
    }

    if (opcion == 1) {
        int danio = calcularDanio(DANIO_ESPADA_MIN, DANIO_ESPADA_MAX, "Jugador", "Enemigo");
        *ep -= danio;
    } else if (opcion == 2) {
        if (*pociones > 0) {
            *hp += CURA_POCION;
            if (*hp > MAX_VIDA) *hp = MAX_VIDA; // Limitar la vida máxima
            *pociones -= 1;
            printf("🍷 Has usado una poción. Vida actual: %d (Pociones restantes: %d)\n", *hp, *pociones);
        } else {
            printf("❌ ¡No te quedan pociones!\n");
        }
    } else if (opcion == 3) {
        printf("Levantas del suelo la matadragones, lograrás blandirla en el siguiente turno.\n");
        *cargarMatadragones = true; // Activar el estado para el próximo turno
    }
}

// Función para el turno del enemigo
void turnoEnemigo(int *hp, int danioMin, int danioMax, const char *nombreEnemigo) {
    int danio = calcularDanio(danioMin, danioMax, nombreEnemigo, "Jugador");
    *hp -= danio;
}

// Función para preguntar si el jugador quiere curarse
void preguntarCuracion(int *hp, int *pociones) {
    int opcion;
    printf("\n¡Has derrotado al enemigo! ¿Quieres curarte antes de enfrentar al siguiente?\n");
    printf("1 - Sí, usar una poción 🍷 (%d disponibles)\n", *pociones);
    printf("2 - No, continuar sin curarme\n");
    printf("Elige una opción: ");
    while (scanf("%d", &opcion) != 1 || (opcion != 1 && opcion != 2)) {
        printf("❌ Opción inválida. Introduce 1 o 2: ");
        while (getchar() != '\n'); // Limpiar el buffer de entrada
    }

    if (opcion == 1 && *pociones > 0) {
        *hp += CURA_POCION;
        if (*hp > MAX_VIDA) *hp = MAX_VIDA; // Limitar la vida máxima
        *pociones -= 1;
        printf("🍷 Has usado una poción. Vida actual: %d (Pociones restantes: %d)\n", *hp, *pociones);
    } else if (opcion == 1 && *pociones == 0) {
        printf("❌ No tienes pociones disponibles. Continuarás sin curarte.\n");
    } else {
        printf("Continúas sin curarte.\n");
    }
}

// Función para generar un nuevo enemigo
void generarEnemigo(const char **nombreEnemigo, int *vidaEnemigo, int *danioEnemigoMin, int *danioEnemigoMax) {
    int tipoEnemigo = rand() % 3; // 0 = lobo, 1 = ogro, 2 = dragón
    switch (tipoEnemigo) {
        case 0:
            *nombreEnemigo = "lobo 🐺";
            *vidaEnemigo = VIDA_LOBO;
            *danioEnemigoMin = DANIO_LOBO_MIN;
            *danioEnemigoMax = DANIO_LOBO_MAX;
            break;
        case 1:
            *nombreEnemigo = "ogro 🧌";
            *vidaEnemigo = VIDA_OGRO;
            *danioEnemigoMin = DANIO_OGRO_MIN;
            *danioEnemigoMax = DANIO_OGRO_MAX;
            break;
        case 2:
            *nombreEnemigo = "dragón 🔥";
            *vidaEnemigo = VIDA_DRAGON;
            *danioEnemigoMin = DANIO_DRAGON_MIN;
            *danioEnemigoMax = DANIO_DRAGON_MAX;
            break;
    }
    printf("\nUn nuevo enemigo aparece: %s\n", *nombreEnemigo);
    printf("Vida del %s: %d\n", *nombreEnemigo, *vidaEnemigo);
}

int main() {
    int hp, ep, pociones = 5, s;
    const char *nombreEnemigo;
    int vidaEnemigo, danioEnemigoMin, danioEnemigoMax;
    bool cargarMatadragones = false; // Variable para controlar el estado de la matadragones

    // Inicializar la semilla para números aleatorios
    srand(time(NULL));

    // Bienvenida al juego
    printf("Hola viajero, bienvenido al Super Duper Malibran RPG 🎮\n");
    printf("Lo primero que debes hacer es elegir tu arma:\n");
    printf("1 - Espada ⚔️\n");
    printf("2 - Solo hay espada 😅\n");
    scanf("%d", &s);

    // Generar el primer enemigo
    generarEnemigo(&nombreEnemigo, &vidaEnemigo, &danioEnemigoMin, &danioEnemigoMax);

    // Inicializar vida del jugador
    inicializarVida(&hp, "Tu vida", MAX_VIDA);

    // Ciclo principal del combate
    do {
        ep = vidaEnemigo; // Restaurar la vida del enemigo al máximo
        do {
            mostrarDivisor(); // Divisor visual antes de cada turno
            mostrarEstado(hp, ep, pociones, nombreEnemigo);
            turnoJugador(&hp, &ep, &pociones, &cargarMatadragones);
            if (ep > 0) {
                turnoEnemigo(&hp, danioEnemigoMin, danioEnemigoMax, nombreEnemigo);
            }
            printf("\nPresiona Enter para continuar...");
            while (getchar() != '\n'); // Limpiar el buffer
            getchar(); // Esperar la entrada del usuario
        } while (hp > 0 && ep > 0);

        // Resultado del combate
        mostrarDivisor();
        if (hp <= 0) {
            printf("\n💀 Has sido derrotado... Game Over.\n");
            break;
        } else {
            printf("\n🏆 ¡Venciste al %s! ¡Eres un héroe!\n", nombreEnemigo);
            preguntarCuracion(&hp, &pociones);
            generarEnemigo(&nombreEnemigo, &vidaEnemigo, &danioEnemigoMin, &danioEnemigoMax);
        }
    } while (true);

    return 0;
}