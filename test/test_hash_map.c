#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <hash_map.h>

typedef struct {
    size_t key;
    size_t value;
    unsigned char used;
} test_entry;

START_TEST(test_create_and_destroy) {
    hash_map* m = map_create(sizeof(test_entry), 8);
    ck_assert_ptr_nonnull(m);
    ck_assert_int_eq(m->count, 0);
    ck_assert(m->capacity >= 8);
    map_destroy(m);
} END_TEST

START_TEST(test_insert_and_get) {
    hash_map* m = map_create(sizeof(test_entry), 8);
    ck_assert_ptr_nonnull(m);

    for (size_t i = 0; i < 10; ++i) {
        test_entry* e = map_get_or_create_entry(m, i,
            sizeof(test_entry), sizeof(size_t));
        ck_assert_ptr_nonnull(e);
        e->value = i * 100;
    }

    for (size_t i = 0; i < 10; ++i) {
        test_entry* e = map_get_entry(m, i, sizeof(test_entry),
             sizeof(size_t));
        ck_assert_ptr_nonnull(e);
        ck_assert_int_eq(e->value, i * 100);
    }
    map_destroy(m);
} END_TEST

START_TEST(test_rehash) {
    hash_map* m = map_create(sizeof(test_entry), 4);
    ck_assert_ptr_nonnull(m);

    size_t before = m->capacity;
    for (size_t i = 0; i < 50; ++i) {
        test_entry* e = map_get_or_create_entry(m, i,
            sizeof(test_entry), sizeof(size_t));
        ck_assert_ptr_nonnull(e);
        e->value = i;
    }

    ck_assert(m->capacity > before);
    map_destroy(m);
} END_TEST

int test_hash_map(void) {
    Suite* suite = suite_create("hash_map");
    TCase* tc_core = tcase_create("core");
    tcase_add_test(tc_core, test_create_and_destroy);
    tcase_add_test(tc_core, test_insert_and_get);
    tcase_add_test(tc_core, test_rehash);
    suite_add_tcase(suite, tc_core);

    SRunner* sr = srunner_create(suite);
    srunner_run_all(sr, CK_NORMAL);
    int failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return failed;   
}
