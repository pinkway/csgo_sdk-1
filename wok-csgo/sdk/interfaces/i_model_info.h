#pragma once

class radian_euler;

class quaternion {
public:
	inline quaternion(void) {}
	inline quaternion(float ix, float iy, float iz, float iw) : x(ix), y(iy), z(iz), w(iw) { }
	inline quaternion(radian_euler const &angle);

	inline void init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f, float iw = 0.0f) { x = ix; y = iy; z = iz; w = iw; }

	bool is_valid() const;
	void invalidate();

	bool operator==(const quaternion &src) const;
	bool operator!=(const quaternion &src) const;

	float* base() { return (float*)this; }
	const float* base() const { return (float*)this; }

	float operator[](int i) const;
	float& operator[](int i);

	float x, y, z, w;
};

class radian_euler {
public:
	radian_euler(void) { }
	radian_euler(float X, float Y, float Z) { x = X; y = Y; z = Z; }
	radian_euler(quaternion const &q);
	radian_euler(qangle_t const &angles);

	inline void init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f) { x = ix; y = iy; z = iz; }

	qangle_t to_angle(void) const;
	bool is_valid() const;
	void invalidate();

	inline float *base() { return &x; }
	inline const float *base() const { return &x; }

	float operator[](int i) const;
	float& operator[](int i);

	float x, y, z;
};

class ALIGN16 quaternion_aligned : public quaternion {
public:
	inline quaternion_aligned(void) {};
	inline quaternion_aligned(float x, float y, float z, float w) {
		init(x, y, z, w);
	}
public:
	explicit quaternion_aligned(const quaternion &in) {
		init(in.x, in.y, in.z, in.w);
	}

	quaternion_aligned& operator=(const quaternion &in) {
		init(in.x, in.y, in.z, in.w);
		return *this;
	}
};

struct mstudiobbox_t {
	int		bone;
	int		group;
	vec3_t	min;
	vec3_t	max;
	int		hitboxnameindex;
	vec3_t  rotation;
	float	radius;
	int		pad[4];

	const char* get_hitbox_name() {
		if (hitboxnameindex == 0)
			return "";

		return ((char*)this) + hitboxnameindex;
	}
};

struct mstudiohitboxset_t {
	int sznameindex;

	inline char * const get_name() const {
		return ((char*)this) + sznameindex;
	}

	int numhitboxes;
	int hitboxindex;

	inline mstudiobbox_t* get_hitbox(int i) const {
		return (mstudiobbox_t*)(((byte*)this) + hitboxindex) + i;
	}
};

struct mstudiobone_t {
	int					sznameindex;
	inline char * const get_name() const { return ((char *)this) + sznameindex; }
	int		 			parent;
	int					bonecontroller[6];

	vec3_t				pos;
	quaternion			quat;
	radian_euler		rot;

	vec3_t				posscale;
	vec3_t				rotscale;

	matrix3x4_t			pose_to_bone;
	quaternion			alignment;
	int					flags;
	int					proctype;
	int					procindex;
	mutable int			physicsbone;
	inline void*		get_procedure() const { if (procindex == 0) return 0; else return (void*)(((byte*)this) + procindex); };
	int					surfacepropidx;
	inline char* const  pszSurfaceProp() const { return ((char*)this) + surfacepropidx; }
	inline int			GetSurfaceProp() const { return surfacepropLookup; }

	int					contents;
	int					surfacepropLookup;
	int					unused[7];
};

struct mstudiobonecontroller_t {
	int					bone;
	int					type;
	float				start;
	float				end;
	int					rest;
	int					inputfield;
	int					unused[8];
};

struct mstudioattachment_t {
	int					sznameindex;
	inline char* const	get_name() const { return ((char*)this) + sznameindex; }
	unsigned int		flags;
	int					localbone;
	matrix3x4_t			local;
	int					unused[8];
};

struct studiohdr_t {
	mstudiohitboxset_t* get_hitbox_set(int i) const {
		return (mstudiohitboxset_t*)(((byte*)this) + hitboxsetindex) + i;
	};

	inline mstudiobbox_t* get_hitbox(int i, int set) const {
		mstudiohitboxset_t const *s = get_hitbox_set(set);
		if (!s)
			return 0;

		return s->get_hitbox(i);
	};

	inline int get_hitbox_count(int set) const {
		mstudiohitboxset_t const *s = get_hitbox_set(set);
		if (!s)
			return 0;

		return s->numhitboxes;
	};

	inline mstudiobone_t* get_bone(int i) const { return (mstudiobone_t *)(((byte *)this) + boneindex) + i; };
	inline mstudioattachment_t* get_local_attachment(int i) const { return (mstudioattachment_t *)(((byte *)this) + localattachmentindex) + i; };

	int id;
	int version;
	int checksum;
	char name[64];
	int length;

	vec3_t eyeposition;
	vec3_t illumposition;
	vec3_t hull_min;
	vec3_t hull_max;
	vec3_t view_bbmin;
	vec3_t view_bbmax;

	int flags;
	int numbones;
	int boneindex;

	int numbonecontrollers;
	int bonecontrollerindex;
	int numhitboxsets;
	int hitboxsetindex;

	int numlocalanim;
	int localanimindex;
	int numlocalseq;
	int localseqindex;

	mutable int activitylistversion;
	mutable int eventsindexed;

	int numtextures;
	int textureindex;
	int numcdtextures;
	int cdtextureindex;

	int numskinref;
	int numskinfamilies;
	int skinindex;

	int numbodyparts;
	int bodypartindex;
	int numlocalattachments;
	int localattachmentindex;

	int numlocalnodes;
	int localnodeindex;
	int localnodenameindex;

	int numflexdesc;
	int flexdescindex;
	int numflexcontrollers;
	int flexcontrollerindex;
	int numflexrules;
	int flexruleindex;
	int numikchains;
	int ikchainindex;
	int nummouths;
	int mouthindex;
	int numlocalposeparameters;
	int localposeparamindex;
	int surfacepropindex;

	int keyvalueindex;
	int keyvaluesize;

	int numlocalikautoplaylocks;
	int localikautoplaylockindex;

	float mass;
	int contents;
	int numincludemodels;
	int includemodelindex;

	mutable void *virtualModel;
	int szanimblocknameindex;

	int numanimblocks;
	int animblockindex;
	mutable void *animblockModel;
	int bonetablebynameindex;

	void *pVertexBase;
	void *pIndexBase;
	unsigned char constdirectionallightdot;
	unsigned char rootLOD;
	unsigned char numAllowedRootLODs;
	unsigned char unused[1];
	int unused4;
	int numflexcontrollerui;
	int flexcontrolleruiindex;
	int unused3[2];
	int studiohdr2index;
	int unused2[1];

	studiohdr_t() {}
private:
	studiohdr_t(const studiohdr_t &vOther);

	friend struct virtualmodel_t;
};

class virtualgroup_t {
public:
	void* cache;
	const studiohdr_t* get_studio_hdr() const {
		return (studiohdr_t*)cache;
	}

	c_utl_vector<int> m_bone_map;
	c_utl_vector<int> m_master_bone;
	c_utl_vector<int> m_master_seq;
	c_utl_vector<int> m_master_anim;
	c_utl_vector<int> m_master_attachment;
	c_utl_vector<int> m_master_pose;
	c_utl_vector<int> m_master_node;
};

struct virtualsequence_t {
	int	flags;
	int activity;
	int group;
	int index;
};

struct virtualgeneric_t {
	int group;
	int index;
};

struct virtualmodel_t {
	char							pad0[8];
	c_utl_vector<virtualsequence_t> m_seq;
	c_utl_vector<virtualgeneric_t>	m_anim;
	c_utl_vector<virtualgeneric_t>	m_attachment;
	c_utl_vector<virtualgeneric_t>	m_pose;
	c_utl_vector<virtualgroup_t>	m_group;
	c_utl_vector<virtualgeneric_t>	m_node;
	c_utl_vector<virtualgeneric_t>	m_iklock;
	c_utl_vector<unsigned short>	m_autoplay_sequences;
};

class c_studio_hdr {
public:
	mutable studiohdr_t*						m_studio_hdr;
	mutable virtualmodel_t*						m_v_model; 
	char										pad0[4];
	mutable c_utl_vector<const studiohdr_t*>	m_studio_hdr_cache;
	mutable int									m_num_frame_unlock_counter;
	int*										m_frame_unlock_counter;
	char										pad1[8];
	c_utl_vector<int>							m_bone_flags;
	c_utl_vector<int>							m_bone_parent;
	inline int									numbones() const { return m_studio_hdr->numbones; };
	inline mstudiobone_t*						get_bone(int i) const { return m_studio_hdr->get_bone(i); };
	inline virtualmodel_t*						get_virtual_model() const { return m_v_model; };

	int	get_num_attachments() const {
		if (m_studio_hdr->numincludemodels == 0) {
			return m_studio_hdr->numlocalattachments;
		}

		virtualmodel_t* model = get_virtual_model();

		return model->m_attachment.count();
	}

	const studiohdr_t* group_studio_hdr(int i) {
		const studiohdr_t* hdr = m_studio_hdr_cache[i];

		if (!hdr) {
			virtualgroup_t* group = &m_v_model->m_group[i];
			hdr = group->get_studio_hdr();
		}

		return hdr;
	}

	const mstudioattachment_t &get_attachment(int i) {
		if (m_v_model == NULL) {
			return *m_studio_hdr->get_local_attachment(i);
		}

		const studiohdr_t* hdr = group_studio_hdr(m_v_model->m_attachment[i].group);

		return *hdr->get_local_attachment(m_v_model->m_attachment[i].index);
	}

	int	get_attachment_bone(int i) {
		const mstudioattachment_t &attachment = get_attachment(i);

		virtualmodel_t *model = get_virtual_model();
		if (model) {
			virtualgroup_t *group = &model->m_group[model->m_attachment[i].group];
			int bone = group->m_master_bone[attachment.localbone];
			if (bone == -1)
				return 0;

			return bone;
		}
		return attachment.localbone;
	}
};

struct model_t;

class i_model_info {
public:
	VFUNC(get_model(int index), 1, model_t*(__thiscall*)(void*, int), index)
	VFUNC(get_model_index(const char* name), 2, int(__thiscall*)(void*, const char*), name)
	VFUNC(get_model_name(const model_t* model), 3, const char*(__thiscall*)(void*, const model_t*), model)
	VFUNC(get_studio_model(const model_t* model), 32, studiohdr_t*(__thiscall*)(void*, const model_t*), model)
};