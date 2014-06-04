#ifndef GW2LIB_H
#define GW2LIB_H

#include <Windows.h>
#include <string>
#include <vector>
#include <utility>

#pragma comment(lib, "gw2lib.lib")

struct PrimitiveDiffuseMesh;
namespace GameData {
	struct CharacterData;
	struct AgentData;
}
namespace GW2 {
	enum Profession {
		PROFESSION_GUARDIAN = 1,
		PROFESSION_WARRIOR,
		PROFESSION_ENGINEER,
		PROFESSION_RANGER,
		PROFESSION_THIEF,
		PROFESSION_ELEMENTALIST,
		PROFESSION_MESMER,
		PROFESSION_NECROMANCER,
		PROFESSION_NONE
	};
	enum Attitude {
		ATTITUDE_FRIENDLY = 0,
		ATTITUDE_HOSTILE,
		ATTITUDE_INDIFFERENT,
		ATTITUDE_NEUTRAL
	};
	enum AgentCategory {
		AGENT_CATEGORY_CHAR = 0,
		AGENT_CATEGORY_DYNAMIC,
		AGENT_CATEGORY_KEYFRAMED
	};
	enum AgentType {
		AGENT_TYPE_CHAR = 0,
		AGENT_TYPE_GADGET = 9,
		AGENT_TYPE_GADGET_ATTACK_TARGET = 10,
		AGENT_TYPE_ITEM = 14
	};
}

namespace GW2LIB
{
	/*void LogPlain(const char *format, ...);
	#define LOG_PLAIN(format,...) GW2LIB::LogPlain(format, __VA_ARGS__)
	#ifdef _DEBUG
	void LogDebug(const char *file, const char *func, int line, const char *format, ...);
	void LogError(const char *file, const char *func, int line, const char *format, ...);
	#define LOG_DBG(format,...) GW2LIB::LogDebug(__FILE__, __FUNCTION__, __LINE__, format, __VA_ARGS__)
	#define LOG_ERR(format,...) GW2LIB::LogError(__FILE__, __FUNCTION__, __LINE__, format, __VA_ARGS__)
	#else
	void LogError(const char *format, ...);
	#define LOG_DBG(format,...)
	#define LOG_ERR(format,...) GW2LIB::LogError(format, __VA_ARGS__);
	#endif*/

	// derive from this and override as needed
	class Main
	{
	public:
		Main();

		HINSTANCE getModuleHandle() const;

		// callback that will recieve console input
		virtual void cbConIn(const std::string& input) { }

		// is called on initialization. if returning false, the dll will detach
		virtual bool init() { return true; }
		// is called continuously sequenially while running. if returning false, the dll will detach
		virtual bool step() { Sleep(25); return true; }
		// is called after internal cleanup is done. the ESP callback is guaranteed to not run anymore during this
		virtual void cleanup() { }
	};

	class Agent;
	class Character;
	struct Vector3 {
		Vector3() { }
		Vector3(float x, float y, float z) : x(x), y(y), z(z) { }
		float x, y, z;
	};
	struct Matrix4x4 {
		float m[4][4];
	};

	//////////////////////////////////////////////////////////////////////////
	// # general functions
	//////////////////////////////////////////////////////////////////////////
	// lets you create a new thread. its save using all functions
	// usage: "void myThread() {}" "NewThread(myThread);"
	void NewThread(void(*)());

	// prints debug string to console. usage: like printf
	void DbgOut(std::string, ...);

	// registers a callback to be used for a custom esp
	// use draw functions inside the callback function
	void EnableEsp(void(*)());

	void ResetConsole(int cellsX = 80, int cellsY = 20);


	//////////////////////////////////////////////////////////////////////////
	// # game classes
	//////////////////////////////////////////////////////////////////////////
	// represents a general game object
	class Agent {
	public:
		Agent();
		Agent(const Agent &);
		Agent &operator= (const Agent &);
		bool operator== (const Agent &);

		bool IsValid() const;

		bool BeNext();
		void BeSelf();

		Character GetCharacter() const;

		GW2::AgentCategory GetCategory() const;
		GW2::AgentType GetType() const;
		int GetAgentId() const;

		Vector3 GetPos() const;
		float GetRot() const;

		GameData::AgentData *m_ptr;
		size_t iterator = 0;
	};
	// represents advanced game objects like players and monsters
	class Character {
	public:
		Character();
		Character(const Character &);
		Character &operator= (const Character &);
		bool operator== (const Character &);

		bool IsValid() const;

		bool BeNext();
		void BeSelf();

		Agent GetAgent() const;

		bool IsAlive() const;
		bool IsDowned() const;
		bool IsControlled() const;
		bool IsPlayer() const;
		bool IsInWater() const;
		bool IsMonster() const;
		bool IsMonsterPlayerClone() const;

		int GetLevel() const;
		int GetScaledLevel() const;
		int GetWvwSupply() const;

		float GetCurrentHealth() const;
		float GetMaxHealth() const;
		float GetCurrentEndurance() const;
		float GetMaxEndurance() const;

		GW2::Profession GetProfession() const;
		GW2::Attitude GetAttitude() const;

		std::string GetName() const;

		GameData::CharacterData *m_ptr;
	};


	//////////////////////////////////////////////////////////////////////////
	// # game functions
	//////////////////////////////////////////////////////////////////////////
	Character GetOwnCharacter();
	Agent GetOwnAgent();
	Agent GetAutoSelection();
	Agent GetHoverSelection();
	Agent GetLockedSelection();
	Vector3 GetMouseInWorld();
	int GetCurrentMapId();
	Vector3 GetCameraPosition();
	Vector3 GetViewVector();
	float GetFieldOfViewY();


	//////////////////////////////////////////////////////////////////////////
	// # draw functions
	//////////////////////////////////////////////////////////////////////////
	// all "draw" functions are only usable in callback function defined with "EnableEsp"

	void DrawLine(float x, float y, float x2, float y2, DWORD color);
	void DrawLineProjected(Vector3 pos1, Vector3 pos2, DWORD color);
	void DrawRect(float x, float y, float w, float h, DWORD color);
	void DrawRectFilled(float x, float y, float w, float h, DWORD color);
	void DrawCircle(float mx, float my, float r, DWORD color);
	void DrawCircleFilled(float mx, float my, float r, DWORD color);

	// circles are drawn parallel to xy-plane
	void DrawCircleProjected(Vector3 pos, float r, DWORD color);
	void DrawCircleFilledProjected(Vector3 pos, float r, DWORD color);

	// returns false when projected position is not on screen
	bool WorldToScreen(Vector3 in, float *outX, float *outY);

	float GetWindowWidth();
	float GetWindowHeight();

	//////////////////////////////////////////////////////////////////////////
	// # complex drawing classes
	//////////////////////////////////////////////////////////////////////////

	class Texture {
	public:
		Texture();
		bool Init(std::string file);
		bool Init(const void *buffer, size_t size);
		void Draw(float x, float y, float w, float h) const;
	private:
		Texture(const Texture &t) { }
		Texture &operator= (const Texture &t) { }
		const void *m_ptr;
	};

	class Font {
	public:
		Font();
		bool Init(int size, std::string name);
		void Draw(float x, float y, DWORD color, std::string format, ...) const;
	private:
		Font(const Font &f) { }
		Font &operator= (const Font &f) { }
		const void *m_ptr;
	};

	// limitation of this: completly ignores depth checks
	// what is drawn last, is on front
	class PrimitiveDiffuse {
	public:
		PrimitiveDiffuse();
		~PrimitiveDiffuse();
		// if indices is empty, primitive is not drawn indexed
		bool Init(std::vector<std::pair<Vector3, DWORD>> vertices, std::vector<unsigned int> indices, bool triangleStrip);
		void SetTransforms(std::vector<Matrix4x4> transforms);
		void AddTransform(Matrix4x4 transform);
		void Draw() const;
	private:
		PrimitiveDiffuse(const PrimitiveDiffuse &p) { }
		PrimitiveDiffuse &operator= (const PrimitiveDiffuse &p) { }
		PrimitiveDiffuseMesh *m_ptr;
	};
}

#endif // GW2LIB_H