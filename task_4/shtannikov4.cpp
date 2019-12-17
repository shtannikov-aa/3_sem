#include <cstdlib>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
using namespace std;

double f(double x) {
    return x*x*x;
}

mutex m;
long long sum = 0;
void monte(long long nsteps, double xmin, double xmax, double ymin, double ymax) {
    drand48_data d;
    srand48_r(rand(), &d);
    long long res = 0;
    for(long long i = 0; i < nsteps; i++) {
        double r1,r2;
        drand48_r(&d, &r1);
        drand48_r(&d, &r2);
        double x = xmin + r1 * (xmax - xmin);
        double y = ymin + r2 * (ymax - ymin);
        if(f(x) > y)
            res++;
    }
    m.lock();
    sum += res;
    m.unlock();
}

long long mtime() {
	return chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

int main() {
    long long npoints = 75694080;
    double xmin = -1, xmax = 2, ymin = -1, ymax = 8;
    for(long long i = 1; i < 0x8000; i++) {
        if(npoints % i != 0)
            continue;
        sum = 0;
        long long t0 = mtime();

        vector<thread> threads;
        for(int j = 0; j < i; j++)
            threads.push_back(thread(monte, npoints / i, xmin, xmax, ymin, ymax));
        for(int j = 0; j < i; j++)
            threads[j].join();
        threads.clear();

        cout << "threads: " << i << ' ';
        cout << "time(ms): " << mtime() - t0 << ' ';
        cout << "result: " << ymin * (xmax - xmin) + (ymax - ymin) * (xmax - xmin) * sum / npoints << ' ';
        cout << endl;
    }
}
/*
Results:
threads: 1 time(ms): 1461 result: 3.75057 
threads: 2 time(ms): 737 result: 3.74842 
threads: 3 time(ms): 517 result: 3.74904 
threads: 4 time(ms): 379 result: 3.74985 
threads: 5 time(ms): 445 result: 3.75183 
threads: 6 time(ms): 390 result: 3.75065 
threads: 7 time(ms): 381 result: 3.74937 
threads: 8 time(ms): 345 result: 3.74743 
threads: 10 time(ms): 383 result: 3.7486 
threads: 11 time(ms): 373 result: 3.75002 
threads: 12 time(ms): 358 result: 3.75022 
threads: 14 time(ms): 357 result: 3.74841 
threads: 15 time(ms): 364 result: 3.7504 
threads: 16 time(ms): 348 result: 3.74894 
threads: 20 time(ms): 350 result: 3.74903 
threads: 21 time(ms): 352 result: 3.74847 
threads: 22 time(ms): 342 result: 3.74928 
threads: 24 time(ms): 345 result: 3.75094 
threads: 28 time(ms): 342 result: 3.7492 
threads: 30 time(ms): 347 result: 3.74869 
threads: 32 time(ms): 366 result: 3.74935 
threads: 33 time(ms): 348 result: 3.74928 
threads: 35 time(ms): 357 result: 3.74805 
threads: 40 time(ms): 348 result: 3.74584 
threads: 42 time(ms): 351 result: 3.75087 
threads: 44 time(ms): 351 result: 3.74994 
threads: 48 time(ms): 347 result: 3.7492 
threads: 55 time(ms): 403 result: 3.74866 
threads: 56 time(ms): 390 result: 3.75048 
threads: 60 time(ms): 358 result: 3.74738 
threads: 64 time(ms): 378 result: 3.74866 
threads: 66 time(ms): 355 result: 3.74899 
threads: 70 time(ms): 353 result: 3.75151 
threads: 77 time(ms): 360 result: 3.75018 
threads: 80 time(ms): 358 result: 3.75067 
threads: 84 time(ms): 382 result: 3.75057 
threads: 88 time(ms): 347 result: 3.7513 
threads: 96 time(ms): 359 result: 3.75028 
threads: 105 time(ms): 376 result: 3.75025 
threads: 110 time(ms): 399 result: 3.74979 
threads: 112 time(ms): 366 result: 3.75049 
threads: 120 time(ms): 357 result: 3.75036 
threads: 128 time(ms): 352 result: 3.74649 
threads: 132 time(ms): 397 result: 3.74969 
threads: 140 time(ms): 362 result: 3.74836 
threads: 154 time(ms): 352 result: 3.75091 
threads: 160 time(ms): 348 result: 3.74754 
threads: 165 time(ms): 349 result: 3.74978 
threads: 168 time(ms): 351 result: 3.74938 
threads: 176 time(ms): 354 result: 3.75074 
threads: 192 time(ms): 341 result: 3.74825 
threads: 210 time(ms): 345 result: 3.75017 
threads: 220 time(ms): 349 result: 3.75016 
threads: 224 time(ms): 347 result: 3.74887 
threads: 231 time(ms): 363 result: 3.74982 
threads: 240 time(ms): 354 result: 3.75212 
threads: 256 time(ms): 342 result: 3.74908 
threads: 264 time(ms): 343 result: 3.74952 
threads: 280 time(ms): 345 result: 3.75157 
threads: 308 time(ms): 351 result: 3.74996 
threads: 320 time(ms): 349 result: 3.75098 
threads: 330 time(ms): 353 result: 3.75099 
threads: 336 time(ms): 349 result: 3.74627 
threads: 352 time(ms): 347 result: 3.75117 
threads: 384 time(ms): 348 result: 3.74893 
threads: 385 time(ms): 393 result: 3.7513 
threads: 420 time(ms): 393 result: 3.74968 
threads: 440 time(ms): 356 result: 3.75092 
threads: 448 time(ms): 365 result: 3.74877 
threads: 462 time(ms): 370 result: 3.75008 
threads: 480 time(ms): 356 result: 3.75091 
threads: 512 time(ms): 368 result: 3.74913 
threads: 528 time(ms): 351 result: 3.74882 
threads: 560 time(ms): 369 result: 3.7496 
threads: 616 time(ms): 359 result: 3.74981 
threads: 640 time(ms): 356 result: 3.74852 
threads: 660 time(ms): 354 result: 3.74971 
threads: 672 time(ms): 377 result: 3.75005 
threads: 704 time(ms): 459 result: 3.7515 
threads: 768 time(ms): 445 result: 3.75059 
threads: 770 time(ms): 347 result: 3.7484 
threads: 840 time(ms): 351 result: 3.75142 
threads: 880 time(ms): 350 result: 3.7503 
threads: 896 time(ms): 350 result: 3.7482 
threads: 924 time(ms): 360 result: 3.75046 
threads: 960 time(ms): 353 result: 3.75053 
threads: 1024 time(ms): 353 result: 3.74942 
threads: 1056 time(ms): 364 result: 3.74954 
threads: 1120 time(ms): 369 result: 3.7525 
threads: 1155 time(ms): 354 result: 3.74999 
threads: 1232 time(ms): 369 result: 3.748 
threads: 1280 time(ms): 363 result: 3.75175 
threads: 1320 time(ms): 366 result: 3.75223 
threads: 1344 time(ms): 368 result: 3.74921 
threads: 1408 time(ms): 357 result: 3.74906 
threads: 1536 time(ms): 377 result: 3.74954 
threads: 1540 time(ms): 380 result: 3.75177 
threads: 1680 time(ms): 421 result: 3.74956 
threads: 1760 time(ms): 388 result: 3.75068 
threads: 1792 time(ms): 369 result: 3.74996 
threads: 1848 time(ms): 374 result: 3.74952 
threads: 1920 time(ms): 382 result: 3.74927 
threads: 2048 time(ms): 378 result: 3.74814 
threads: 2112 time(ms): 380 result: 3.74965 
threads: 2240 time(ms): 377 result: 3.74773 
threads: 2310 time(ms): 398 result: 3.75067 
threads: 2464 time(ms): 415 result: 3.75036 
threads: 2560 time(ms): 506 result: 3.75081 
threads: 2640 time(ms): 413 result: 3.75052 
threads: 2688 time(ms): 398 result: 3.7476 
threads: 2816 time(ms): 520 result: 3.75059 
threads: 3072 time(ms): 423 result: 3.75077 
threads: 3080 time(ms): 428 result: 3.75139 
threads: 3360 time(ms): 416 result: 3.74931 
threads: 3520 time(ms): 415 result: 3.74961 
threads: 3584 time(ms): 412 result: 3.75055 
threads: 3696 time(ms): 419 result: 3.74901 
threads: 3840 time(ms): 429 result: 3.74876 
threads: 4096 time(ms): 434 result: 3.74997 
threads: 4224 time(ms): 459 result: 3.74879 
threads: 4480 time(ms): 460 result: 3.75255 
threads: 4620 time(ms): 447 result: 3.75058 
threads: 4928 time(ms): 456 result: 3.75046 
threads: 5120 time(ms): 467 result: 3.75069 
threads: 5280 time(ms): 462 result: 3.75199 
threads: 5376 time(ms): 480 result: 3.75138 
threads: 5632 time(ms): 538 result: 3.74862 
threads: 6144 time(ms): 547 result: 3.74924 
threads: 6160 time(ms): 479 result: 3.75318 
threads: 6720 time(ms): 503 result: 3.74592 
threads: 7040 time(ms): 507 result: 3.75108 
threads: 7168 time(ms): 503 result: 3.74927 
threads: 7392 time(ms): 525 result: 3.75004 
threads: 7680 time(ms): 506 result: 3.74714 
threads: 8192 time(ms): 514 result: 3.75038 
threads: 8448 time(ms): 512 result: 3.74889 
threads: 8960 time(ms): 522 result: 3.75117 
threads: 9240 time(ms): 535 result: 3.74951 
threads: 9856 time(ms): 526 result: 3.75032 
threads: 10240 time(ms): 515 result: 3.74974 
threads: 10560 time(ms): 513 result: 3.74834 
threads: 10752 time(ms): 527 result: 3.74828 
threads: 11264 time(ms): 536 result: 3.75046 
threads: 12288 time(ms): 548 result: 3.74934 
threads: 12320 time(ms): 523 result: 3.74892 
threads: 13440 time(ms): 550 result: 3.75021 
threads: 14080 time(ms): 560 result: 3.75003 
threads: 14336 time(ms): 555 result: 3.74892 
threads: 14784 time(ms): 558 result: 3.75113 
threads: 15360 time(ms): 552 result: 3.74784 
threads: 16384 time(ms): 571 result: 3.74948 
threads: 16896 time(ms): 620 result: 3.75007 
threads: 17920 time(ms): 599 result: 3.74756 
threads: 18480 time(ms): 616 result: 3.75291 
threads: 19712 time(ms): 600 result: 3.74907 
threads: 20480 time(ms): 602 result: 3.74984 
threads: 21120 time(ms): 612 result: 3.7517 
threads: 21504 time(ms): 611 result: 3.74912 
threads: 22528 time(ms): 642 result: 3.74883 
threads: 24576 time(ms): 703 result: 3.74934 
threads: 24640 time(ms): 676 result: 3.75209 
threads: 26880 time(ms): 730 result: 3.74934 
threads: 28160 time(ms): 769 result: 3.75198 
threads: 28672 time(ms): 746 result: 3.75272 
threads: 29568 time(ms): 759 result: 3.752 
threads: 30720 time(ms): 796 result: 3.74903 
*/