#pragma once

namespace memory {
	struct protect_t {
		protect_t(LPVOID address, uint32_t size, DWORD flags) : m_address(address), m_size(size), m_flags(0) {
			VirtualProtect(m_address, m_size, flags, &m_flags);
		}

		~protect_t() {
			VirtualProtect(m_address, m_size, m_flags, &m_flags);
		}

		LPVOID m_address;
		uint32_t m_size;
		DWORD m_flags;
	};

	__forceinline uint32_t get_vtable_length(uintptr_t* table) {
		auto length = uint32_t{};
		for (length = 0; table[length]; length++)
			if (IS_INTRESOURCE(table[length]))
				break;

		return length;
	}

	struct hook_t {
		hook_t() = default;
		hook_t(uintptr_t ptr) : m_vtable(reinterpret_cast<uintptr_t**>(ptr)), m_table_length(0), m_orig(nullptr), m_replace(nullptr) { init(); };
		hook_t(void* ptr) : m_vtable(reinterpret_cast<uintptr_t**>(ptr)), m_table_length(0), m_orig(nullptr), m_replace(nullptr) { init(); };

		bool init() {
			if (!m_vtable)
				return false;

			auto protect = protect_t(m_vtable, sizeof(uintptr_t), PAGE_READWRITE);

			m_orig = *m_vtable;

			m_table_length = get_vtable_length(m_orig);
			if (!m_table_length)
				return false;

			m_replace = std::make_unique<uintptr_t[]>(m_table_length + 1);

			std::memset(m_replace.get(),
				NULL,
				m_table_length * sizeof(uintptr_t) + sizeof(uintptr_t));

			std::memcpy(&m_replace[1],
				m_orig,
				m_table_length * sizeof(uintptr_t));

			std::memcpy(m_replace.get(),
				&m_orig[-1],
				sizeof(uintptr_t));

			*m_vtable = &m_replace[1];

			return true;
		}

		template<typename T>
		void hook(const uint16_t index, T replace_function) {
			if (index < 0 
				|| index > m_table_length)
				return;

			m_replace[index + 1] = reinterpret_cast<uintptr_t>(replace_function);
		}

		template<typename T>
		T get_original(const uint16_t index) {
			if (index < 0 
				|| index > m_table_length)
				return nullptr;

			return reinterpret_cast<T>(m_orig[index]);
		}

		void unhook(const uint16_t index) {
			if (index < 0
				|| index > m_table_length)
				return;

			m_replace[index + 1] = m_orig[index];
		}

		void unhook() {
			if (!m_orig)
				return;

			auto protect = protect_t(m_vtable, sizeof(uintptr_t), PAGE_READWRITE);

			*m_vtable = m_orig;
			m_orig = nullptr;
		}

		uintptr_t** m_vtable;
		uint16_t m_table_length;
		uintptr_t* m_orig;
		std::unique_ptr<uintptr_t[]> m_replace;
	};

	__forceinline uintptr_t get_module_handle(const uintptr_t module, const uintptr_t process = 0) {
		MODULEENTRY32 entry;
		entry.dwSize = sizeof(MODULEENTRY32);

		auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process);

		if (Module32First(snapshot, &entry)) {
			while (Module32Next(snapshot, &entry)) {
				std::string name = entry.szModule;
				std::transform(name.begin(), name.end(), name.begin(), ::tolower);

				if (fnv1a_rt(name.c_str()) == module) {
					CloseHandle(snapshot);
					return reinterpret_cast<uintptr_t>(entry.hModule);
				}
			}
		}

		CloseHandle(snapshot);
		return 0;
	}

	__forceinline uint8_t* find_sig(const uintptr_t offset, const char* signature, const uintptr_t range = 0) {
		static auto sig_to_bytes = [](const char* pattern) -> std::vector<int> {
			std::vector<int> bytes;
			const auto start = const_cast<char*>(pattern);
			const auto end = const_cast<char*>(pattern) + strlen(pattern);

			for (auto current = start; current < end; current++) {
				if (*current == '?') {
					current++;

					if (*current == '?')
						current++;

					bytes.push_back(-1);
				}
				else
					bytes.push_back(static_cast<int>(strtoul(current, &current, 0x10)));
			}

			return bytes;
		};

		auto sig_bytes = sig_to_bytes(signature);
		const auto size = sig_bytes.size();
		const auto data = sig_bytes.data();

		const auto scan_bytes = reinterpret_cast<uint8_t*>(offset);

		for (auto i = 0ul; i < range - size; i++) {
			auto found = true;

			for (auto j = 0ul; j < size; j++)
				if (scan_bytes[i + j] != data[j] && data[j] != -1) {
					found = false;
					break;
				}

			if (found)
				return &scan_bytes[i];
		}

		return nullptr;
	}

	__forceinline uint8_t* find_module_sig(const uintptr_t name, const char* signature) {
		auto module = get_module_handle(name);

		if (module) {
			const auto dos_header = PIMAGE_DOS_HEADER(module);
			const auto nt_headers = PIMAGE_NT_HEADERS(reinterpret_cast<uint8_t*>(module) + dos_header->e_lfanew);

			return find_sig(module, signature, nt_headers->OptionalHeader.SizeOfImage);
		}

		return nullptr;
	}

	template<typename T>
	__forceinline T capture_interface(const char* module_name, const char* interface_name) {
		auto module_handle = GetModuleHandleA(module_name);
		if (!module_handle)
			return nullptr;

		auto create_interface_fn = reinterpret_cast<T(*)(const char*, int*)>(GetProcAddress(module_handle, _("CreateInterface")));
		if (!create_interface_fn)
			return nullptr;

		return reinterpret_cast<T>(create_interface_fn(interface_name, nullptr));
	}

	template<typename T>
	__forceinline T get_vfunc(void* base, int index) { return (*static_cast<T**>(base))[index]; }
}

#define SIG(module_name, sig) memory::find_module_sig(fnv1a(module_name), _ot(sig))

#define _INTERFACE(val, type, module_name, interface_name) val = memory::capture_interface<type*>(module_name, interface_name);
#define _INTERFACE_OFFSET(val, type, ptr, index, add) val = **reinterpret_cast<type***>((*reinterpret_cast<uintptr_t**>(ptr))[index] + add);
#define _INTERFACE_SIG(val, type, module_name, sig, add) { static const auto addr = SIG(module_name, sig); val = *reinterpret_cast<type**>(addr + add); }
#define _PINTERFACE_SIG(val, type, module_name, sig, add) { static const auto addr = SIG(module_name, sig); val = **reinterpret_cast<type***>(addr + add); }

#define VFUNC(func, index, type, ...) auto func { return memory::get_vfunc<type>(this, index)(this, __VA_ARGS__); };
#define CUSTOM_VFUNC(func, module_name, sig, type, ...) auto func { static const auto addr = SIG(module_name, sig); return reinterpret_cast<type>(addr)(this, __VA_ARGS__); };