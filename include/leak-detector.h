// Standard malloc ve free fonksiyonlarını kendi yazacağımız fonksiyonlara yönlendirir (#define).
#ifndef LEAK_DETECTOR_H
#define LEAK_DETECTOR_H

#include <stddef.h>

// Standart malloc ve free çağrılarını kendi sarmalayıcılarımıza yönlendiriyoruz.
// __FILE__ ve __LINE__ sayesinden hangi dosyada ve satırda çağrıldığını alıyoruz.
#define malloc(size) my_malloc(size, __FILE__, __LINE__)
#define free(ptr) my_free(ptr)

// Fonksiyon Bildirimleri
void* my_malloc(size_t size, const char* file, int line);
void my_free(void* ptr);
void check_leaks(void);

#endif // LEAK_DETECTOR_H