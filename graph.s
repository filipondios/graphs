
struc hash_map ;; Generic hashmap
	.entries  resq 1 ;; void**
	.count    resq 1 ;; size_t
	.capacity resq 1 ;; size_t
endstruc

struc entry_src ;; Source entry 
	.key resq 1 ;; char*
	.map resq 1 ;; hash_map*
endstruc

struc entry_dst ;; Destination entry
	.key    resq 1 ;; char*
	.weight resq 1 ;; size_t
endstruc

section .bss
sizeof_hash_map  resb hash_map
sizeof_entry_src resb entry_src
sizeof_entry_dst resb entry_dst

section .text

;; hash_map* create_map(size_t entry_size, size_t initial_capacity)
;; void map_free(hash_map* map)
;; void map_set(hash_map* map, void* entry)
;; void* map_get(hash_map* map, char* key)
