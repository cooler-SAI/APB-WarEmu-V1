ALTER TABLE `quests`
ADD COLUMN `PrevQuest`  int(11) NOT NULL AFTER `Choice`;