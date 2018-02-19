/**
 * Kostra programu pro 3. projekt IZP 2015/16
 *
 * Jednoducha shlukova analyza
 * Complete linkage
 * http://is.muni.cz/th/172767/fi_b/5739129/web/web/clsrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX
#include <string.h>
#include <errno.h>

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    c->size = 0;
    if (cap == 0) {
        c->capacity = 0;
        c->obj = NULL;
    }
    else {
        c->capacity = ((c->obj = malloc(cap * sizeof(struct obj_t))) == NULL) ? 0 : cap;
    }
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    free(c->obj);
    init_cluster(c, 0);
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    if (c->size == c->capacity) {
        c = resize_cluster(c, c->capacity + CLUSTER_CHUNK);
    }
    c->obj[(c->size)++] = obj;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    for (int i = 0; i < c2->size; i++) {
        append_cluster(c1, c2->obj[i]);
    }
    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    struct cluster_t to_del = carr[idx];
    carr[idx] = carr[--narr];
    carr[narr] = to_del;
    clear_cluster(&carr[narr]);

    return narr;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    return (sqrtf(powf(o1->x - o2->x, 2) + powf(o1->y - o2->y, 2)));
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    float new_dist = 0, max_dist = -1;

    for (int i = 0; i < c1->size; i++) {
        for (int j = 0; j < c2->size; j++) {
            new_dist = obj_distance(&c1->obj[i], &c2->obj[j]);

            if (new_dist > max_dist) {
                max_dist = new_dist;
            }
        }
    }
    return max_dist;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);

    float new_dist = 0, min_dist = INFINITY;

	for (int i = 0; i < narr - 1; i++) {
		for (int j = i + 1; j < narr; j++) {
			new_dist = cluster_distance(&carr[i], &carr[j]);

			if (new_dist < min_dist) {
				min_dist = new_dist;
				*c1 = i;
				*c2 = j;
			}
		}
	}
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = a;
    const struct obj_t *o2 = b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Does the file \"%s\" exist?\n", filename);
        return 0;
    }

    int to_load = 0;
    if ((fscanf(file, "count=%d", &to_load) == 0) || to_load <= 0 || (fgetc(file) != '\n')) {
        fprintf(stderr, "There is no 'count=N' line where N > 0.\n");
        to_load = 0;
    }

    int loaded = 0;
    if (to_load > 0) {
        *arr = malloc(sizeof(struct cluster_t)*to_load);
        if (*arr == NULL) {
            fprintf(stderr, "%s\n", "Something went wrong while allocating memory.\n");
        }
        else {
            struct obj_t temp;
            int lines = 0;

            while (to_load-- != 0) {

                if ((fscanf(file, "%d %f %f", &temp.id, &temp.x, &temp.y) != 3) || (temp.id < 0 || temp.x < 0 || temp.x > 1000 || temp.y < 0 || temp.y > 1000) || (fgetc(file) != '\n')) {
                    fprintf(stderr, "A strange object was read on line: %d\nTerminating...\n", lines + 2);
                    while (--lines >= 0) {
                        clear_cluster(&(*arr)[lines]);
                    }
                    free(*arr);
                    *arr = NULL;
                    lines = 0;
                    fprintf(stderr, "Done.\n");
                    break;
                }
                else {
                    init_cluster(&(*arr)[lines], 1);
                    append_cluster(&(*arr)[lines], temp);
                    lines++;
                }
            }
            loaded = lines;
        }
    }

    if (fclose(file) == EOF) {
        fprintf(stderr, "Something went wrong while closing file \"%s\".\n", filename);
    }
    return loaded;
}



/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
    int to_print = 0;
    if (argc == 2) {
        to_print = 1;
    }
    else if (argc == 3) {
        char *trash;
        to_print = strtol(argv[2], &trash, 10);
        if (to_print <= 0 || (strcmp(trash, "") != 0) || (errno == ERANGE)) {
            fprintf(stderr, "Your second argument must be a number above 0.\n");
            return 1;
        }
    }
    else {
        fprintf(stderr, "Wrong arguments.\n");
        return 1;
    }

    struct cluster_t *clusters = NULL;
    int loaded = load_clusters(argv[1], &clusters);

	if (clusters == NULL || loaded < to_print) {
		fprintf(stderr, "It's not possible to create %d clusters with %d objects.\n", to_print, loaded);
	}
    else {
        int first = 0, second = 0;
        while (loaded > to_print) {
            find_neighbours(clusters, loaded, &first, &second);
            merge_clusters(&clusters[first], &clusters[second]);
            loaded = remove_cluster(clusters, loaded, second);
        }
        print_clusters(clusters, to_print);
        to_print = 0;
    }

    while (--loaded >= 0) {
        clear_cluster(&clusters[loaded]);
    }
    free(clusters);

    return (to_print == 0 ? 0 : 1);
}
