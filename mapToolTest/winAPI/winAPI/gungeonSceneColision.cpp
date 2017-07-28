#include "stdafx.h"
#include "gungeonScene.h"

void gungeonScene::objectColision(void)
{
	tagCharacter ptemp;
	vector<Object*> ovtemp;
	RECT temprc = RectMake(0,0,0,0);
	ptemp = _cm->getPlayer()->getCharacterData();
	ovtemp = _om->getObjectvector();

	for (int i = 0; i < ovtemp.size(); ++i)
	{
		if (IntersectRect(&temprc, &ptemp.rc, &ovtemp[i]->getObjectData().crc) &&
			(ovtemp[i]->getObjectData().type == DOOR_LENGTH || ovtemp[i]->getObjectData().type == DOOR_WIDTH))
		{
			if (!ovtemp[i]->getOpen() && !ovtemp[i]->getAction())
			{
				SOUNDMANAGER->play("dooropen");
				ovtemp[i]->getAction() = true;
				if (temprc.right == ovtemp[i]->getObjectData().crc.right || temprc.bottom == ovtemp[i]->getObjectData().crc.bottom)
				{
					ovtemp[i]->getObjectData().frameY = 1;
				}

			}
		}
	}
}