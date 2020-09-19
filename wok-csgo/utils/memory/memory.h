#pragma once

namespace memory {
	struct protect_t {
		protect_t(LPVOID address, uint32_t size, DWORD flags) {
			m_size = size;
			m_address = address;

			VirtualProtect(m_address, m_size, flags, &m_flags);
		}

		~protect_t() { VirtualProtect(m_address, m_size, m_flags, &m_flags); }

		DWORD m_flags = 0ul;
		uint32_t m_size = 0u;
		LPVOID m_address = nullptr;
	};

	__forceinline uint32_t get_vtable_length(uintptr_t* table) {
		auto i = 0u;

		for (; table[i]; i++) {
			if (IS_INTRESOURCE(table[i]))
				break;
		}

		return i;
	}

	struct hook_t {
		hook_t() = default;

		hook_t(uintptr_t ptr) {
			m_vtable = reinterpret_cast<uintptr_t**>(ptr);

			init();
		};

		hook_t(void* ptr) {
			m_vtable = reinterpret_cast<uintptr_t**>(ptr);

			init();
		};

		__forceinline bool init() {
			if (!m_vtable)
				return false;

			const auto protect = protect_t(m_vtable, sizeof(uintptr_t), PAGE_READWRITE);

			m_original = *m_vtable;

			m_table_length = get_vtable_length(m_original);
			if (!m_table_length)
				return false;

			m_replace = std::make_unique<uintptr_t[]>(m_table_length + 1);

			std::memset(m_replace.get(), 0, m_table_length * sizeof(uintptr_t) + sizeof(uintptr_t));

			std::memcpy(&m_replace[1], m_original, m_table_length * sizeof(uintptr_t));

			std::memcpy(m_replace.get(), &m_original[-1], sizeof(uintptr_t));

			*m_vtable = &m_replace[1];

			return true;
		}

		template<typename T>
		__forceinline void hook(uint32_t index, T replace_function) {
			if (index < 0u
				|| index > m_table_length)
				return;

			m_replace[index + 1u] = reinterpret_cast<uintptr_t>(replace_function);
		}

		template<typename T>
		__forceinline T get_original(uint32_t index) {
			if (index < 0
				|| index > m_table_length)
				return nullptr;

			return reinterpret_cast<T>(m_original[index]);
		}

		__forceinline void unhook(uint32_t index) {
			if (index < 0u
				|| index > m_table_length)
				return;

			m_replace[index + 1u] = m_original[index];
		}

		__forceinline void unhook() {
			if (!m_original)
				return;

			const auto protect = protect_t(m_vtable, sizeof(uintptr_t), PAGE_READWRITE);

			*m_vtable = m_original;
			m_original = nullptr;
		}

		uint32_t m_table_length = 0u;
		uintptr_t** m_vtable = nullptr;
		uintptr_t* m_original = nullptr;
		std::unique_ptr<uintptr_t[]> m_replace = nullptr;
	};

	struct address_t {
		address_t() = default;
		address_t(uint8_t* ptr) { m_ptr = ptr; };
		address_t(uintptr_t* ptr) { m_ptr = reinterpret_cast<uint8_t*>(ptr); };

		__forceinline operator uint8_t*() { return m_ptr; }

		__forceinline operator void*() { return reinterpret_cast<void*>(m_ptr); }

		__forceinline uint8_t* get() { return m_ptr; }

		template<typename T>
		__forceinline T cast() { return reinterpret_cast<T>(m_ptr); }

		__forceinline uint8_t* offset(int offset) const { return m_ptr + offset; };

		__forceinline address_t self_offset(int offset) {
			m_ptr += offset;

			return *this;
		}

		__forceinline uint8_t* jmp(int offset = 0x1) const { return m_ptr + offset + sizeof(uintptr_t) + *reinterpret_cast<int*>(m_ptr + offset); }

		__forceinline address_t self_jmp(int offset = 0x1) {
			m_ptr = jmp(offset);

			return *this;
		}

		uint8_t* m_ptr = nullptr;
	};

	__forceinline uintptr_t get_module_handle(uint32_t module, uint32_t process = 0) {
		MODULEENTRY32 entry;

		entry.dwSize = sizeof(MODULEENTRY32);

		const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process);

		uintptr_t ret = 0u;

		if (Module32First(snapshot, &entry)) {
			while (Module32Next(snapshot, &entry)) {
				auto name = std::string(entry.szModule);

				std::transform(name.begin(), name.end(), name.begin(), ::tolower);

				if (FNV1A_RT(name.c_str()) != module)
					continue;

				ret = reinterpret_cast<uintptr_t>(entry.hModule);
				break;
			}
		}

		CloseHandle(snapshot);

		return ret;
	}

	__forceinline std::vector<int> sig_to_bytes(const char* sig) {
		const auto start = const_cast<char*>(sig);
		const auto end = const_cast<char*>(sig) + strlen(sig);

		auto ret = std::vector<int>();

		for (auto it = start; it < end; it++) {
			if (*it == '?') {
				it++;

				if (*it == '?') {
					it++;
				}

				ret.push_back(-1);
			}
			else {
				ret.push_back(static_cast<int>(strtoul(it, &it, 0x10)));
			}
		}

		return ret;
	}

	__forceinline address_t find_sig(uint32_t offset, const char* sig, uint32_t range = 0u) {
		const auto sig_bytes = sig_to_bytes(sig);
		const auto size = sig_bytes.size();
		const auto data = sig_bytes.data();

		const auto scan_bytes = reinterpret_cast<uint8_t*>(offset);

		for (auto i = 0u; i < range - size; i++) {
			auto found = true;

			for (auto j = 0u; j < size; j++) {
				if (data[j] == -1
					|| scan_bytes[i + j] == data[j])
					continue;

				found = false;
				break;
			}

			if (found)
				return &scan_bytes[i];
		}

		return address_t();
	}

	__forceinline address_t find_module_sig(uint32_t hash, const char* sig) {
		const auto module = get_module_handle(hash);
		if (!module)
			return address_t();

		const auto dos_header = reinterpret_cast<PIMAGE_DOS_HEADER>(module);
		const auto nt_headers = reinterpret_cast<PIMAGE_NT_HEADERS>(reinterpret_cast<uint8_t*>(module) + dos_header->e_lfanew);

		return find_sig(module, sig, nt_headers->OptionalHeader.SizeOfImage);
	}

	template <typename T>
	__forceinline T capture_interface(uint32_t hash, const char* interface_name) {
		const auto module_handle = get_module_handle(hash);
		if (!module_handle)
			return nullptr;

		const auto create_interface_fn = reinterpret_cast<T(__cdecl*)(const char*, int*)>(GetProcAddress(reinterpret_cast<HMODULE>(module_handle), _("CreateInterface")));
		if (!create_interface_fn)
			return nullptr;

		return reinterpret_cast<T>(create_interface_fn(interface_name, nullptr));
	}

	template<typename T>
	__forceinline T get_vfunc(void* base, int index) { return (*static_cast<T**>(base))[index]; }
}

#define SIG(module_name, sig) memory::find_module_sig(FNV1A(module_name), _(sig))

#define _INTERFACE(value, type, module_name, interface_name) value = memory::capture_interface<type*>(FNV1A(module_name), _(interface_name));
#define _INTERFACE_OFFSET(value, type, ptr, index, add) value = **reinterpret_cast<type***>((*reinterpret_cast<uintptr_t**>(ptr))[index] + add);
#define _INTERFACE_SIG(value, type, module_name, sig, add) { value = *SIG(module_name, sig).self_offset(add).cast<type**>(); }
#define _PINTERFACE_SIG(value, type, module_name, sig, add) { value = **SIG(module_name, sig).self_offset(add).cast<type***>(); }

#define VFUNC(func, index, type, ...) __forceinline auto func { return memory::get_vfunc<type>(this, index)(this, __VA_ARGS__); };
#define VFUNC_SIG(func, module_name, sig, type, ...) __forceinline auto func { static const auto fn = SIG(module_name, sig).cast<type>(); return fn(this, __VA_ARGS__); };