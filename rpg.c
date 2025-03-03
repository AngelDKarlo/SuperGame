#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>  // Para rand(), srand()
#include <time.h>    // Para time()
//hola mundo
// Constantes para el juego
#define MAX_VIDA_INICIAL 100
#define DANIO_ESPADA_MIN 2
#define DANIO_ESPADA_MAX 30
#define DANIO_LOBO_MIN 10
#define DANIO_LOBO_MAX 20
#define DANIO_OGRO_MIN 15
#define DANIO_OGRO_MAX 25
#define DANIO_DRAGON_MIN 25
#define DANIO_DRAGON_MAX 60
#define CURA_POCION_BASE 40
#define CURA_POCION_MAX 100
#define PROBABILIDAD_CURA_MEJORADA 40 // Probabilidad en % de que la poción cure más
#define VIDA_LOBO 75
#define VIDA_OGRO 150
#define VIDA_DRAGON 450
#define RECOMPENSA_MONEDAS_TIENDA 10
#define COSTO_ARMADURA 20
#define COSTO_POCION_FUERZA 20
#define MEJORA_VIDA_ARMADURA 50
#define MEJORA_DANIO_POCION 20
#define COSTO_FILO_ESPADA 10
#define MEJORA_FILO_ESPADA 10

void clearconsole(){
    printf("\033[H\033[J");
}
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
void mostrarEstado(int hp, int ep, int pociones, bool escudoActivo, int turnosEscudo, const char *nombreEnemigo, int monedas, int filoEspada) {
    printf("\n--- Estado Actual ---\n");
    if (escudoActivo) {
        printf("Tu vida: %d (ESCUDO ACTIVO - %d turnos restantes) | ", hp, turnosEscudo);
    } else {
        printf("Tu vida: %d | ", hp);
    }
    printf("Vida del %s: %d | Pociones: %d | Monedas: %d | Filo de la Espada: +%d daño\n", nombreEnemigo, ep, pociones, monedas, filoEspada * MEJORA_FILO_ESPADA);
}

// Función para calcular el daño con rango y crítico
int calcularDanio(int min, int max, int danioExtra, const char *atacante, const char *objetivo) {
    int danio = min + rand() % (max - min + 1) + danioExtra; // Daño aleatorio entre min y max + daño extra
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
void turnoJugador(int *hp, int *ep, int *pociones, bool *cargarMatadragones, bool *escudoDisponible, bool *escudoActivo, int *turnosEscudo, int danioExtra, int filoEspada) {
    int opcion;

    // Si la matadragones fue activada en el turno anterior, atacar automáticamente
    if (*cargarMatadragones) {
        printf("¡Ataque automático con la matadragones!\n");
        int danio = calcularDanio(DANIO_ESPADA_MIN, DANIO_ESPADA_MAX, danioExtra + (filoEspada * MEJORA_FILO_ESPADA), "Jugador", "Enemigo");
        *ep -= danio;
        *cargarMatadragones = false; // Reiniciar el estado
        return;
    }

    // Menú normal del jugador
    printf("\n¿Qué deseas hacer?\n");
    printf("1 - Espadazo 🗡️ (haces entre %d y %d de daño)\n", DANIO_ESPADA_MIN + danioExtra + (filoEspada * MEJORA_FILO_ESPADA), DANIO_ESPADA_MAX + danioExtra + (filoEspada * MEJORA_FILO_ESPADA));
    printf("2 - Usar poción 🍷 (cura hasta %d de vida, tienes %d pociones)\n", CURA_POCION_MAX, *pociones);
    printf("3 - Usar la matadragones\n");
    if (*escudoDisponible) {
        printf("4 - Activar ESCUDO INVENCIBLE (protege contra 3 ataques enemigos)\n");
    }
    printf("Elige una opción: ");
    while (scanf("%d", &opcion) != 1 || (opcion < 1 || opcion > 4) || (!*escudoDisponible && opcion == 4)) {
        printf("❌ Opción inválida. Introduce una opción válida: ");
        while (getchar() != '\n'); // Limpiar el buffer de entrada
    }

    if (opcion == 1) {
        int danio = calcularDanio(DANIO_ESPADA_MIN, DANIO_ESPADA_MAX, danioExtra + (filoEspada * MEJORA_FILO_ESPADA), "Jugador", "Enemigo");
        *ep -= danio;
    } else if (opcion == 2) {
        if (*pociones > 0) {
            int curacion = CURA_POCION_BASE;
            if (rand() % 100 < PROBABILIDAD_CURA_MEJORADA) { // Probabilidad de curación mejorada
                curacion = CURA_POCION_BASE + rand() % (CURA_POCION_MAX - CURA_POCION_BASE + 1);
                printf("🍷 ¡POCIÓN MEJORADA! Curas %d puntos de vida.\n", curacion);
            } else {
                printf("🍷 Usaste una poción. Curas %d puntos de vida.\n", curacion);
            }
            *hp += curacion;
            *pociones -= 1;
            printf("Vida actual: %d (Pociones restantes: %d)\n", *hp, *pociones);
        } else {
            printf("❌ ¡No te quedan pociones!\n");
        }
    } else if (opcion == 3) {
        printf("Levantas del suelo la matadragones, lograrás blandirla en el siguiente turno.\n");
        *cargarMatadragones = true; // Activar el estado para el próximo turno
    } else if (opcion == 4 && *escudoDisponible) {
        printf("🛡️ ¡ESCUDO INVENCIBLE ACTIVADO! Estarás protegido contra los próximos 3 ataques enemigos.\n");
        *escudoActivo = true;
        *turnosEscudo = 4;
        *escudoDisponible = false; // El escudo solo puede usarse una vez
    }
}

// Función para el turno del enemigo
void turnoEnemigo(int *hp, int danioMin, int danioMax, const char *nombreEnemigo, bool *escudoActivo, int *turnosEscudo) {
    if (*escudoActivo) {
        printf("🛡️ ¡El ESCUDO INVENCIBLE te protege del ataque del %s!\n", nombreEnemigo);
        (*turnosEscudo)--;
        if (*turnosEscudo == 0) {
            *escudoActivo = false;
            printf("🛡️ ¡El ESCUDO INVENCIBLE ha expirado!\n");
        }
    } else {
        int danio = calcularDanio(danioMin, danioMax, 0, nombreEnemigo, "Jugador");
        *hp -= danio;
    }
}

// Función para la tienda
void tienda(int *monedas, int *vidaMaxima, int *danioExtra, int *hp, int *filoEspada) {
    int opcion;
    printf("\n🎉 ¡Bienvenido a la tienda! Tienes %d monedas.\n", *monedas);
    printf("1 - Comprar ARMADURA (%d monedas): Aumenta tu vida máxima en %d puntos.\n", COSTO_ARMADURA, MEJORA_VIDA_ARMADURA);
    printf("2 - Comprar POCIÓN DE FUERZA (%d monedas): Aumenta tu daño base en %d puntos.\n", COSTO_POCION_FUERZA, MEJORA_DANIO_POCION);
    printf("3 - Mejorar FILO DE LA ESPADA (%d monedas): Aumenta tu daño base en +%d puntos por cada nivel.\n", COSTO_FILO_ESPADA, MEJORA_FILO_ESPADA);
    printf("4 - No comprar nada y continuar.\n");
    printf("Elige una opción: ");
    while (scanf("%d", &opcion) != 1 || (opcion < 1 || opcion > 4)) {
        printf("❌ Opción inválida. Introduce 1, 2, 3 o 4: ");
        while (getchar() != '\n'); // Limpiar el buffer de entrada
    }

    if (opcion == 1 && *monedas >= COSTO_ARMADURA) {
        *vidaMaxima += MEJORA_VIDA_ARMADURA;
        *monedas -= COSTO_ARMADURA;
        printf("🛡️ ¡Has comprado la ARMADURA! Tu vida máxima ahora es %d.\n", *vidaMaxima);
    } else if (opcion == 1 && *monedas < COSTO_ARMADURA) {
        printf("❌ No tienes suficientes monedas para comprar la ARMADURA.\n");
    } 
    else if (opcion == 2 && *monedas >= COSTO_POCION_FUERZA) {
        *danioExtra += MEJORA_DANIO_POCION;
        *monedas -= COSTO_POCION_FUERZA;
        printf("💪 ¡Has comprado la POCIÓN DE FUERZA! Tu daño base ahora es %d.\n", DANIO_ESPADA_MIN + *danioExtra + (*filoEspada * MEJORA_FILO_ESPADA));
    } else if (opcion == 2 && *monedas < COSTO_POCION_FUERZA) {
        printf("❌ No tienes suficientes monedas para comprar la POCIÓN DE FUERZA.\n");
    } else if (opcion == 3 && *monedas >= COSTO_FILO_ESPADA) {
        (*filoEspada)++;
        *monedas -= COSTO_FILO_ESPADA;
        printf("⚔️ ¡Has mejorado el FILO DE LA ESPADA! Nivel de Filo: %d (+%d de daño).\n", *filoEspada, *filoEspada * MEJORA_FILO_ESPADA);
    } else if (opcion == 3 && *monedas < COSTO_FILO_ESPADA) {
        printf("❌ No tienes suficientes monedas para mejorar el FILO DE LA ESPADA.\n");
    } else {
        printf("Continúas sin comprar nada.\n");
    }

    // Restaurar vida y dar monedas adicionales
    *hp = *vidaMaxima;
    *monedas += RECOMPENSA_MONEDAS_TIENDA;
    printf("❤️ ¡Tu vida ha sido restaurada a %d! Ahora tienes %d monedas.\n", *hp, *monedas);
}

// Función para generar un nuevo enemigo
void generarEnemigo(const char **nombreEnemigo, int *vidaEnemigo, int *danioEnemigoMin, int *danioEnemigoMax, int *recompensaMonedas) {
    int tipoEnemigo = rand() % 3; // 0 = lobo, 1 = ogro, 2 = dragón
    bool dragon;
    switch (tipoEnemigo) {
        case 0:
            *nombreEnemigo = "lobo 🐺";
            *vidaEnemigo = VIDA_LOBO;
            *danioEnemigoMin = DANIO_LOBO_MIN;
            *danioEnemigoMax = DANIO_LOBO_MAX;
            *recompensaMonedas = 10;
            break;
        case 1:
            *nombreEnemigo = "ogro 🧌";
            *vidaEnemigo = VIDA_OGRO;
            *danioEnemigoMin = DANIO_OGRO_MIN;
            *danioEnemigoMax = DANIO_OGRO_MAX;
            *recompensaMonedas = 15;
            break;
        case 2:
            *nombreEnemigo = "dragón 🔥";
            *vidaEnemigo = VIDA_DRAGON;
            *danioEnemigoMin = DANIO_DRAGON_MIN;
            *danioEnemigoMax = DANIO_DRAGON_MAX;
            *recompensaMonedas = 20;
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
    bool escudoDisponible = true;    // Variable para controlar si el escudo puede ser activado
    bool escudoActivo = false;       // Variable para controlar si el escudo está activo
    int turnosEscudo = 0;            // Contador de turnos restantes del escudo
    int monedas = 20;                // Dinero del jugador (inicia con 20 monedas)
    int vidaMaxima = MAX_VIDA_INICIAL; // Vida máxima del jugador
    int danioExtra = 0;              // Daño extra del jugador
    int filoEspada = 0;              // Nivel de Filo de la Espada
    int enemigosDerrotados = 0;      // Contador de enemigos derrotados
    int recompensaMonedas = 0;       // Recompensa por derrotar al enemigo

    // Inicializar la semilla para números aleatorios
    srand(time(NULL));

    // Bienvenida al juego
    printf("Hola viajero, bienvenido al Super Duper Malibran RPG 🎮\n");
    printf("Lo primero que debes hacer es elegir tu arma:\n");
    printf("1 - Espada ⚔️\n");
    printf("2 - Solo hay espada 😅\n");
    scanf("%d", &s);

    // Tienda al inicio del juego
    tienda(&monedas, &vidaMaxima, &danioExtra, &hp, &filoEspada);

    // Generar el primer enemigo
    generarEnemigo(&nombreEnemigo, &vidaEnemigo, &danioEnemigoMin, &danioEnemigoMax, &recompensaMonedas);

    // Inicializar vida del jugador
    inicializarVida(&hp, "Tu vida", vidaMaxima);

    // Ciclo principal del combate
    do {
        ep = vidaEnemigo; // Restaurar la vida del enemigo al máximo
        do {
            mostrarDivisor(); // Divisor visual antes de cada turno
            mostrarEstado(hp, ep, pociones, escudoActivo, turnosEscudo, nombreEnemigo, monedas, filoEspada);
            turnoJugador(&hp, &ep, &pociones, &cargarMatadragones, &escudoDisponible, &escudoActivo, &turnosEscudo, danioExtra, filoEspada);
            if (ep > 0) {
                turnoEnemigo(&hp, danioEnemigoMin, danioEnemigoMax, nombreEnemigo, &escudoActivo, &turnosEscudo);
            }
            printf("\nPresiona Enter para continuar...");
            while (getchar() != '\n'); // Limpiar el buffer
            getchar(); // Esperar la entrada del usuario
            clearconsole();
        } while (hp > 0 && ep > 0);

        // Resultado del combate
        mostrarDivisor();
        if (hp <= 0) {
            printf("\n💀 Has sido derrotado... Game Over.\n");
            break;
        } else {
            printf("\n🏆 ¡Venciste al %s! ¡Eres un héroe!\n", nombreEnemigo);
            monedas += recompensaMonedas;
            printf("💰 Recibes %d monedas. Total de monedas: %d\n", recompensaMonedas, monedas);
            enemigosDerrotados++;

            // Entrar a la tienda después de derrotar a 2 enemigos
            if (enemigosDerrotados == 2) {
                tienda(&monedas, &vidaMaxima, &danioExtra, &hp, &filoEspada);
                enemigosDerrotados = 0; // Reiniciar el contador
            }

            generarEnemigo(&nombreEnemigo, &vidaEnemigo, &danioEnemigoMin, &danioEnemigoMax, &recompensaMonedas);
        }
    } while (true);

    return 0;
}