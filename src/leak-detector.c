// Tüm kayıt tutma ve raporlama işlemleri burada gerçekleşir.
#include <stdio.h>
#include <stdlib.h>
#include "leak-detector.h"

// Sonsuz döngüye girmemek için bu dosyada makroları iptal ediyoruz.
// Böylece gerçek malloc ve free'yi çağırabileceğiz.
#undef malloc
#undef free

// Defterdeki her bir kayıt (Bağlı Liste Düğümü)
typedef struct BlockInfo {
    void* ptr;               // Ayrılan belleğin adresi
    size_t size;             // Bayt cinsinden boyutu
    const char* file;        // Hangi dosyada çağrıldı?
    int line;                // Hangi satırda çağrıldı?
    struct BlockInfo* next;  // Bir sonraki kayıt
} BlockInfo;

// Defterimizin başı (Başlangıçta boş)
static BlockInfo* head = NULL;

// 1. BELLEK AYIRMA VE DEFTERE EKLEME
void* my_malloc(size_t size, const char* file, int line) {
    // Gerçek belleği sistemden istiyoruz
    void* ptr = malloc(size);
    if (!ptr) return NULL; // Bellek ayrılamadıysa dur

    // Deftere eklemek için yeni bir düğüm oluşturuyoruz
    BlockInfo* node = (BlockInfo*)malloc(sizeof(BlockInfo));
    node->ptr = ptr;
    node->size = size;
    node->file = file;
    node->line = line;
    
    // Listelerin başına ekliyoruz (Head insertion)
    node->next = head;
    head = node;

    return ptr;
}

// 2. BELLEK İADE ETME VE DEFTERDEN SİLME
void my_free(void* ptr) {
    if (!ptr) return;

    BlockInfo** current = &head;

    // Listede ilgili adresi arıyoruz
    while (*current) {
        if ((*current)->ptr == ptr) {
            BlockInfo* to_delete = *current;
            *current = (*current)->next; // Düğümü listeden çıkar
            
            free(to_delete->ptr); // Gerçek kullanıcı belleğini serbest bırak
            free(to_delete);      // Defterdeki takip düğümünü sil
            return;
        }
        current = &((*current)->next);
    }

    // Eğer listede yoksa geçersiz/çift free yapılmaya çalışılıyordur
    printf("[UYARI] Bilinmeyen veya zaten silinmiş bellek serbest bırakılmaya çalışıldı: %p\n", ptr);
}

// 3. DEFTERİ KONTROL ETME VE RAPORLAMA
void check_leaks(void) {
    BlockInfo* curr = head;
    size_t total_leaked = 0;
    int leak_count = 0;

    printf("\n=========================================\n");
    printf("         MEMORY LEAK DETECTOR RAPORU     \n");
    printf("=========================================\n");

    while (curr) {
        printf("[SIZINTI] Adres: %p | Boyut: %zu bytes | Dosya: %s | Satir: %d\n",
               curr->ptr, curr->size, curr->file, curr->line);
        
        total_leaked += curr->size;
        leak_count++;
        curr = curr->next;
    }

    if (leak_count == 0) {
        printf(" -> TEBRIKLER! Hic bellek sizintisi tespit edilmedi.\n");
    } else {
        printf("-----------------------------------------\n");
        printf(" Toplam Sizinti Sayisi : %d\n", leak_count);
        printf(" Toplam Kayip Bellek   : %zu bytes\n", total_leaked);
    }
    printf("=========================================\n\n");
}