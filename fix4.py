import codecs
import re

def process_file_regex(path, replacements):
    encoding = 'utf-8-sig'
    try:
        with codecs.open(path, 'r', encoding) as f:
            content = f.read()
    except UnicodeDecodeError:
        encoding = 'shift_jis'
        with codecs.open(path, 'r', encoding) as f:
            content = f.read()
            
    for old_pattern, new_text in replacements:
        content = re.sub(old_pattern, new_text, content, flags=re.DOTALL)
        
    with codecs.open(path, 'w', encoding) as f:
        f.write(content)

# 1. ServiceLocator.h
process_file_regex(r'Source\ServiceLocator.h', [
    (r'#pragma once', '#pragma once\r\n#include <vector>'),
    (r'static Player3D\* GetPlayer\(\);', 'static Player3D* GetPlayer();\r\n\tstatic std::vector<Player3D*> GetPlayers();')
])

# 2. ServiceLocator.cpp
new_sl_cpp = """Player3D* ServiceLocator::GetPlayer()
{
	ObjectManager* objMgr = GetObjectManager();
	if (objMgr != nullptr)
	{
		return dynamic_cast<Player3D*>(objMgr->GetObject3DByTag(Object3D::Tag3D_player));
	}
	return nullptr;
}

std::vector<Player3D*> ServiceLocator::GetPlayers()
{
	std::vector<Player3D*> players;
	ObjectManager* objMgr = GetObjectManager();
	if (objMgr != nullptr)
	{
		const auto& list = objMgr->GetObject3DListByTag(Object3D::Tag3D_player);
		for (auto obj : list)
		{
			if (Player3D* p = dynamic_cast<Player3D*>(obj))
			{
				players.push_back(p);
			}
		}
	}
	return players;
}"""

process_file_regex(r'Source\ServiceLocator.cpp', [
    (r'Player3D\* ServiceLocator::GetPlayer\(\)\r?\n\{.*?\r?\n\s*return dynamic_cast<Player3D\*>\(objMgr->GetObject3DByTag\(Object3D::Tag3D_player\)\);\r?\n\s*\}\r?\n\s*return nullptr;\r?\n\}', new_sl_cpp)
])

# 3. Tatumaki.cpp
new_tatumaki = """\tauto players = ServiceLocator::GetPlayers();
\tPlayer3D* p = nullptr;
\tfloat minDistSq = -1.0f;
\tfor (auto player : players)
\t{
\t\tVECTOR diff = VSub(player->GetPosition(), mPos);
\t\tdiff.y = 0; // x,z distance only
\t\tfloat distSq = VSquareSize(diff);
\t\tif (minDistSq < 0 || distSq < minDistSq)
\t\t{
\t\t\tminDistSq = distSq;
\t\t\tp = player;
\t\t}
\t}

\tif (p != nullptr)
\t{
\t\tVECTOR playerPos = p->GetPosition();
\t\tVECTOR targetDir = VSub(playerPos, mPos);
\t\ttargetDir.y = 0; // x,z only"""

process_file_regex(r'Source\Tatumaki.cpp', [
    (r'auto p = ServiceLocator::GetPlayer\(\);\r?\n\r?\n\s*if \(p != nullptr\)\r?\n\s*\{\r?\n\s*//[^\n]*\r?\n\s*VECTOR playerPos = p->GetPosition\(\);\r?\n\s*VECTOR targetDir = VSub\(playerPos, mPos\);\r?\n\r?\n\s*targetDir\.y = 0;\s*//[^\n]*', new_tatumaki)
])

# 4. Thunder.cpp
new_thunder = """\t\t\t\tauto players = ServiceLocator::GetPlayers();
\t\t\t\tbool playSound = false;
\t\t\t\tfor (auto p : players)
\t\t\t\t{
\t\t\t\t\tVECTOR diff = VSub(p->GetPosition(), mPos);
\t\t\t\t\tif (VSquareSize(diff) < 3000.0f * 3000.0f)
\t\t\t\t\t{
\t\t\t\t\t\tplaySound = true;
\t\t\t\t\t\tbreak;
\t\t\t\t\t}
\t\t\t\t}
\t\t\t\tif (playSound)
\t\t\t\t{
\t\t\t\t\tMaster::mpSoundManager->PlaySE(SoundManager::SE_KAMINARI);
\t\t\t\t}"""

process_file_regex(r'Source\Thunder.cpp', [
    (r'auto p = ServiceLocator::GetPlayer\(\);\r?\n\s*if \(p != nullptr\)\r?\n\s*\{\r?\n\s*//[^\n]*\r?\n\s*VECTOR diff = VSub\(p->GetPosition\(\), mPos\);\r?\n\s*if \(VSquareSize\(diff\) < 3000\.0f \* 3000\.0f\)\r?\n\s*\{\r?\n\s*Master::mpSoundManager->PlaySE\(SoundManager::SE_KAMINARI\);\r?\n\s*\}\r?\n\s*\}', new_thunder)
])

print("Done")
