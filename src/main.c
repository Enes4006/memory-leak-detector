// Bu örnek, bellek sızıntılarını tespit etmek için basit bir C programıdır.
// #include <stdio.h>
// #include "leak-detector.h"

// int main() {
//     printf("Program baslatildi...\n");

//     // Sızıntı 1
//     int* leak_array = (int*)malloc(sizeof(int) * 5); // 20 bytes

//     // Temiz bellek
//     char* clean_str = (char*)malloc(100);             // 100 bytes
//     free(clean_str);

//     // Sızıntı 2
//     double* leak_matrix = (double*)malloc(sizeof(double) * 10); // 80 bytes

//     check_leaks();
//     return 0;
// }


#include <stdio.h>
#include <stdlib.h>
#include "leak-detector.h"

#define MAX_ALLOCATIONS 100

// Kullanıcının oluşturduğu bellek adreslerini geçici tutacağımız yapı
typedef struct {
    void* ptr;
    size_t size;
    int id;
} AllocationRecord;

int main() {
    AllocationRecord records[MAX_ALLOCATIONS];
    int record_count = 0;
    int choice = 0;
    int next_id = 1;

    printf("=========================================\n");
    printf("   INTERAKTIF BELLEK YONETIM PANELI     \n");
    printf("=========================================\n");

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. Bellek Ayir (malloc)\n");
        printf("2. Bellek Serbest Birak (free)\n");
        printf("3. Mevcut Ayrilmis Bellekleri Listele\n");
        printf("4. Memory Leak Raporunu Al ve Cikis Yap\n");
        printf("Seciminiz (1-4): ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Gecersiz giris! Lutfen bir sayi girin.\n");
            break;
        }

        if (choice == 1) {
            // --- BELLEK AYIRMA (MALLOC) ---
            if (record_count >= MAX_ALLOCATIONS) {
                printf("[HATA] Maksimum kayit sinirina ulasildi!\n");
                continue;
            }

            size_t size;
            printf("Kac byte bellek ayirmak istiyorsunuz?: ");
            scanf("%zu", &size);

            void* ptr = malloc(size);
            if (ptr) {
                records[record_count].ptr = ptr;
                records[record_count].size = size;
                records[record_count].id = next_id++;
                
                printf("[BASARILI] ID: %d | Adres: %p | Boyut: %zu bytes bellek ayrildi.\n",
                       records[record_count].id, ptr, size);
                record_count++;
            } else {
                printf("[HATA] Bellek ayrilamadi!\n");
            }

        } else if (choice == 2) {
            // --- BELLEK SERBEST BIRAKMA (FREE) ---
            if (record_count == 0) {
                printf("[UYARI] Serbest birakilacak aktif bellek yok!\n");
                continue;
            }

            printf("\n--- Aktif Ayrilmis Bellekler ---\n");
            for (int i = 0; i < record_count; i++) {
                printf("ID: %d | Adres: %p | Boyut: %zu bytes\n", 
                       records[i].id, records[i].ptr, records[i].size);
            }

            int target_id;
            printf("Silmek istediginiz bellegin ID'sini girin: ");
            scanf("%d", &target_id);

            int found_index = -1;
            for (int i = 0; i < record_count; i++) {
                if (records[i].id == target_id) {
                    found_index = i;
                    break;
                }
            }

            if (found_index != -1) {
                // Free fonksiyonumuzu cagiriyoruz
                free(records[found_index].ptr);
                printf("[BASARILI] ID: %d adresteki bellek serbest birakildi.\n", target_id);

                // Diziden bu kaydi cikariyoruz (Diziyi kaydirarak)
                for (int i = found_index; i < record_count - 1; i++) {
                    records[i] = records[i + 1];
                }
                record_count--;
            } else {
                printf("[HATA] Gecersiz ID girdiniz!\n");
            }

        } else if (choice == 3) {
            // --- AKTIF BELLEKLERI LISTELEME ---
            if (record_count == 0) {
                printf("\n[BILGI] Su anda ayrilmis hicbir bellek yok.\n");
            } else {
                printf("\n--- Su An Heap'te Olan Bellekler (%d adet) ---\n", record_count);
                for (int i = 0; i < record_count; i++) {
                    printf("ID: %d | Adres: %p | Boyut: %zu bytes\n", 
                           records[i].id, records[i].ptr, records[i].size);
                }
            }

        } else if (choice == 4) {
            // --- CIKIS VE RAPOR ---
            printf("\nProgramdan cikiliyor... Sizinti kontrolu yapiliyor.\n");
            break;
        } else {
            printf("Gecersiz secim! Lutfen 1-4 arasinda bir deger girin.\n");
        }
    }

    // Program biterken sızıntı kontrol raporumuzu basıyoruz
    check_leaks();

    return 0;
}