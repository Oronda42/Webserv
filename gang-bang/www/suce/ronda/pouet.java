import java.util.*;
import java.util.List;
import java.awt.*;
import java.math.*;

class Player {
    static final int TYPE_MONSTER = 0;
    static final int TYPE_MY_HERO = 1;
    static final int TYPE_OP_HERO = 2;

    static final String[] song = "Ah-oooh Ah-ooooh You should know that a man with the power of nature Can bring you to the end of your luck And you should know by my stride and the look in my eye That you're about to be massively forced to give up! Chicken attack Chicken attack Watch your back Before it fades to black They might look harmless but they'll kick your non-chicken ass Go chicken go! Go chicken go! Now go, now fly You own the sky Ah-oooh Ah-ooooh With the power of nature you're never alone And you can't let evil run amok Every beast, every tree follows me to the end And you're about to be massively fucked! Chicken attack Chicken attack Watch your back Before it fades to black They might look harmless but they'll kick your non-chicken ass Go chicken go! Go chicken go! Now go, now fly You own the sky You're young and you're hungry Perhaps short on money I give you this chicken today Its eggs for your dinner Its legs for next winter You won't have to steal again Go chicken go! Go chicken go! Now go, now fly You own the sky".split("\\s+");

    static class Entity
    {
        int id;
        int type;
        Point pos;

        int x;
        int y;

        int shieldLife;
        boolean isControlled;
        int health;
        Point dir;
        int vx;
        int vy;

        boolean nearBase;
        int threatFor;
        boolean isTargeted;

        Entity(int id, int type, Point pos, int shieldLife, boolean isControlled, int health, Point dir, boolean nearBase, int threatFor) {
            this.id = id;
            this.type = type;

            this.pos = pos;
            this.x = pos.x;
            this.y = pos.y;

            this.shieldLife = shieldLife;
            this.isControlled = isControlled;
            this.health = health;

            this.dir = dir;
            this.vx = dir.x;
            this.vy = dir.y;

            this.nearBase = nearBase;
            this.threatFor = threatFor;
            this.isTargeted = false;
        }

        public int distance(Entity e)
        { return (int) Math.round(Math.hypot(e.pos.x - this.pos.x, e.pos.y - this.pos.y)); }

        public int distance(Point p)
        { return (int) Math.round(Math.hypot(p.x - this.pos.x, p.y - this.pos.y)); }

        public int distance(Base base)
        { return (int) Math.round(Math.hypot(this.pos.x - base.x, this.pos.y - base.y)); }

        // Returns closest entity from the list to the curent Entity
        public <T extends Entity> T getClosest(List<T> entities)
        {
            if (entities.isEmpty())
            { return null; }

            int closestDist = entities.get(0).distance(this);
            T closestEntity = entities.get(0);
            for (T entity : entities) {
                int dist = entity.distance(this);
                if (dist < closestDist)
                {
                    closestDist = dist;
                    closestEntity = entity;
                }
            }
            return closestEntity;
        }

        // Closest intersection point if entity keep moving in a straight line
        // Litteraly bruteforce, test all possibilities and get closest one
        public <T extends Entity> Point getClosestIntersection(T entity)
        {
            int x = this.x;
            int y = this.y;

            int vx = this.vx;
            int vy = this.vy;

            List<Point> futurePos = new ArrayList<>();

            boolean changedDir = false;

            // List all points entity will go through
            while (x >= 0 && x <= 17630 && y >= 0 && y <= 9000)
            {
                x += vx;
                y += vy;

                // Change direction vector to target base (rotating vector is the simplest)
                if (Math.hypot(0 - x, 0 - y) <= 5000 && !changedDir)
                {
                    int targetX = 0;
                    int targetY = 0;

                    double distance = Math.hypot(x - targetX, y - targetY);
                    double vecX = 0 - x;
                    double vecY = 0 - y;

                    if (vecX != 0 && vecY != 0)
                    {
                        vecX /= distance;
                        vecY /= distance;
                    }

                    // Scale with monster speed
                    vecX *= 400;
                    vecY *= 400;
                
                    vx = (int) Math.round(vecX);
                    vy = (int) Math.round(vecY);

                    changedDir = true;
                }
                else if (Math.hypot(17630 - x, 9000 - y) <= 5000 && !changedDir)
                {
                    int targetX = 17630;
                    int targetY = 9000;

                    double distance = Math.hypot(x - targetX, y - targetY);
                    double vecX = 0 - x;
                    double vecY = 0 - y;

                    if (vecX != 0 && vecY != 0)
                    {
                        vecX /= distance;
                        vecY /= distance;
                    }

                    // Scale with monster speed
                    vecX *= 400;
                    vecY *= 400;

                    vx = (int) Math.round(vecX);
                    vy = (int) Math.round(vecY);

                    changedDir = true;
                }
                else if (Math.hypot(17630 - x, 9000 - y) > 5000 && Math.hypot(0 - x, 0 - y) > 5000)
                {
                    changedDir = false;
                }
                System.err.println("Monster will move at " + x + "; " + y);
                futurePos.add(new Point(x, y));
            }

            for (int i = 1; i <= futurePos.size(); i++) {
                if (entity.distance(futurePos.get(i)) <= 800 * i)
                {
                    
                }
            }
            return (new Point());
        }
    }

    static class Base
    {
        int x;
        int y;

        // Based on top-left base (0;0), get relative position, 100;100 would return either 100;100 or 17530;8900
        Point getRelativePos(Point pos)
        {
            if (this.x < 17630/2)
                return pos;
            else
                return new Point(17630 - pos.x, 9000 - pos.y);
        }

        Point getRelativePos(int x, int y)
        {
            if (this.x < 17630/2)
                return new Point(x, y);
            else
                return new Point(17630 - x, 9000 - y);
        }

        Base(int x, int y)
        { this.x = x; this.y = y; }

        Base(Point p)
        { this.x = p.x; this.y = p.y; }

        public <T extends Entity> T getClosest(List<T> entities)
        {
            if (entities.isEmpty())
            { return null; }

            int closestDist = entities.get(0).distance(this);
            T closestEntity = entities.get(0);
            for (T entity : entities) {
                int dist = entity.distance(this);
                if (dist < closestDist)
                {
                    closestDist = dist;
                    closestEntity = entity;
                }
            }
            return closestEntity;
        }

        public <T extends Entity> int distance(T entity)
        { return (entity.distance(this)); }

        public int distance(Point p)
        { return (int) Math.round(Math.hypot(p.x - this.x, p.y - this.y)); }
    }

    static class Monster extends Entity
    {
        boolean isTargeted;

        Monster(int id, int type, Point pos, int shieldLife, boolean isControlled, int health, Point dir, boolean nearBase, int threatFor)
        {
            super(id, type, pos, shieldLife, isControlled, health, dir, nearBase, threatFor);
            this.isTargeted = false;
        }

        Monster(Monster m)
        {
            super(m.id, m.type, m.pos, m.shieldLife, m.isControlled, m.health, m.dir, m.nearBase, m.threatFor);
            this.isTargeted = m.isTargeted;
        }

        public boolean needHelpToKill(Base base)
        {
            while (this.shieldLife > 0)
            {
                this.x += this.vx;
                this.y += this.vy;
                this.shieldLife--;
            }

            if (base.x > 17630 / 2)
            {
                if (this.x > base.x || this.y > base.y)
                    return true;
            }
            else
            {
                if (this.x < base.x || this.y < base.y)
                    return true;
            }

            return false;
        }
    }

    static class Hero extends Entity
    {
        public int index = 0;

        Hero(int id, int type, Point pos, int shieldLife, boolean isControlled, int health, Point dir, boolean nearBase, int threatFor, int index)
        {
            super(id, type, pos, shieldLife, isControlled, health, dir, nearBase, threatFor);
            this.index = index;
        }

        Hero(Hero m)
        {
            super(m.id, m.type, m.pos, m.shieldLife, m.isControlled, m.health, m.dir, m.nearBase, m.threatFor);
            this.index = m.index;
        }

        public String attack(Monster monster, List<Monster> monsters, String message)
        {
            Monster closest = monster.getClosest(monsters);
            Point target = new Point(monster.pos.x + vx, monster.pos.y + vy);
            if (closest != null && closest.distance(monster) < 1600)
            {
                closest.isTargeted = true;
                Point diff = new Point(monster.pos.x - closest.pos.x, monster.pos.y - closest.pos.y);
                target = new Point(monster.pos.x - diff.x / 2, monster.pos.y - diff.y / 2);
            }
            monster.isTargeted = true;
            return (String.format("MOVE %d %d %s", target.x, target.y, message));
        }

        public String move(Point pos, String message)
        {
            return (String.format("MOVE %d %d %S", pos.x, pos.y, message));
        }

        public String move(int x, int y, String message)
        {
            return (this.move(new Point(x, y), message));
        }

        public <T extends Entity> String shield(T entity, String message)
        {
            entity.shieldLife = 12;
            return String.format("SPELL SHIELD %d %s", entity.id, message);
        }

        public <T extends Entity> String wind(int x, int y, String message)
        {
            return (String.format("SPELL WIND %d %d %s", x, y, message));
        }

        public <T extends Entity> String wind(Base target, String message)
        {
            return (this.wind(target.x, target.y, message));
        }

        public <T extends Entity> String control(int id, int x, int y, String message)
        {
            return (String.format("SPELL CONTROL %d %d %d %s", id, x, y, message));
        }

        public <T extends Entity> String control(int id, Point target, String message)
        {
            return (this.control(id, target.x, target.y, message));
        }

        public String wait(String message)
        {
            return (String.format("WAIT %s", message));
        }

        public <T extends Entity> boolean inSight(List<T> entities)
        {
            return (this.getClosest(entities) != null);
        }

        public <T extends Entity> T closestInSight(List<T> entities)
        {
            return this.getClosest(entities);
        }
    }

    static class Patrol
    {
        private List<Point> points = new ArrayList<>();
        private int next;
        int dir = 1;

        Patrol(List<Point> points)
        { this.points = points; }

        Patrol(Point[] points)
        {
            for (Point point : points) {
                this.points.add(point);
            }
        }

        public void addPoint(Point pt)
        { this.points.add(pt); }

        public Point nextPos(Point currentPos)
        {
            if (points.size() == 0)
                return points.get(0);

            Point nextPt = points.get(next);
            if (Math.hypot(currentPos.x - nextPt.x, currentPos.y - nextPt.y) < 100)
            {
                if (next + dir >= points.size() || next + dir < 0)
                    dir = dir == 1 ? -1 : 1;
                next += dir;
            }
            return (points.get(next));
        }
    }

    // Establish priority for monsters
    static public class DefensePriorityComparator implements Comparator<Monster>
    {
        private Base myBase = new Base(0, 0);

        public DefensePriorityComparator(Base myBase)
        { this.myBase = myBase; }

        @Override
        public int compare(Monster m1, Monster m2)
        {
            // if (m1.threatFor == 1 && m2.threatFor != 1)
            //     return -1;
            // else if (m1.threatFor != 1 && m2.threatFor == 1)
            //     return 1;
            // else
                return m1.distance(myBase) - m2.distance(myBase);
        }
    }

    static public class AttackPriorityComparator implements Comparator<Monster>
    {
        private Base ennemyBase = new Base(0, 0);

        public AttackPriorityComparator(Base ennemyBase)
        { this.ennemyBase = ennemyBase; }

        @Override
        public int compare(Monster m1, Monster m2)
        {
            return m1.distance(ennemyBase) - m2.distance(ennemyBase);
        }
    }

    public static void main(String args[]) {
        Scanner in = new Scanner(System.in);
        // base_x,base_y: The corner of the map representing your base

        Base myBase = new Base(in.nextInt(), in.nextInt());
        Base ennemyBase = new Base(17630 - myBase.x, 9000 - myBase.y); 

        // heroesPerPlayer: Always 3
        int heroesPerPlayer = in.nextInt();     

        // Gameplay variables
        int currentTurn = 0;
        int songIndex = 0;

        Point[] scoutPoints = {
            myBase.getRelativePos(11630, 8000),
            myBase.getRelativePos(5000, 7000),
            myBase.getRelativePos(7500, 2500)
        };

        // game loop
        while (true) {
            currentTurn++;
            int myHealth = in.nextInt(); // Your base health
            int myMana = in.nextInt(); // Ignore in the first league; Spend ten mana to cast a spell
            int ennemyHealth = in.nextInt();
            int ennemyMana = in.nextInt();
            int entityCount = in.nextInt(); // Amount of heros and monsters you can see
            
            List<Hero> myHeroes = new ArrayList<>();
            List<Hero> defenseHeroes = new ArrayList<>(entityCount);
            List<Hero> attackHeroes = new ArrayList<>(entityCount);
            List<Hero> ennemyHeroes = new ArrayList<>(entityCount);
            List<Monster> monsters = new ArrayList<>(entityCount);

            for (int i = 0; i < entityCount; i++) {
                int id = in.nextInt();                              // Unique identifier      
                int type = in.nextInt();                            // 0=monster, 1=your hero, 2=opponent hero    
                Point pos = new Point(in.nextInt(), in.nextInt());  // Position of this entity
                int shieldLife = in.nextInt();                      // Ignore for this league; Count down until shield spell fades      
                boolean isControlled = in.nextInt() == 1;           // Ignore for this league; Equals 1 when this entity is under a control spell        
                int health = in.nextInt();                          // Remaining health of this monster
                Point dir = new Point(in.nextInt(), in.nextInt());  // Trajectory of this monster      
                boolean nearBase = in.nextInt() == 1;               // 0=monster with no target yet, 1=monster targeting a base        
                int threatFor = in.nextInt();                       // Given this monster's trajectory, is it a threat to 1=your base, 2=your opponent's base, 0=neither       
                
                switch (type) {
                    case TYPE_MONSTER:
                        monsters.add(new Monster(id, type, pos, shieldLife, isControlled, health, dir, nearBase, threatFor));
                        break;
                    case TYPE_MY_HERO:
                        int index = id;
                        if (id >= 3)
                            index -= 3;
                        myHeroes.add(new Hero(id, type, pos, shieldLife, isControlled, health, dir, nearBase, threatFor, index));
                        if (id == 0 || id == 2 || id == 3 || id == 5)
                            attackHeroes.add(new Hero(id, type, pos, shieldLife, isControlled, health, dir, nearBase, threatFor, index));
                        else
                            defenseHeroes.add(new Hero(id, type, pos, shieldLife, isControlled, health, dir, nearBase, threatFor, index));
                        break;
                    case TYPE_OP_HERO:
                        ennemyHeroes.add(new Hero(id, type, pos, shieldLife, isControlled, health, dir, nearBase, threatFor, -1));
                        break;
                }
            }

            // Initialize priority list
            List<Monster> defensePriorityList = new ArrayList<>(monsters.size());
            for (Monster monster : monsters)
            { defensePriorityList.add(new Monster(monster)); }

            List<Monster> attackPriorityList = new ArrayList<>(monsters.size());
            for (Monster monster : monsters)
            { attackPriorityList.add(new Monster(monster)); }

            // Sort by priority
            defensePriorityList.sort(new DefensePriorityComparator(myBase));
            attackPriorityList.sort(new AttackPriorityComparator(ennemyBase));
            
            for(int i = 0; i < defensePriorityList.size() && i < 3; i++)
            {
                Monster monster = defensePriorityList.get(i);
                System.err.println(String.format("Defense %d: (%d; %d)", i, monster.pos.x, monster.pos.y));
            }

            for(int i = 0; i < attackPriorityList.size() && i < 3; i++)
            {
                Monster monster = attackPriorityList.get(i);
                System.err.println(String.format("Attack %d: (%d; %d)", i, monster.pos.x, monster.pos.y));
            }


            // Eeach hero attacks the closest monster with higher priority, if any hero haven't attacked, do default actions
            // action[0] = hero 0, action[1] = hero 1 and action[2] = hero 2
            List<String> actions = new ArrayList<>();
            for (int i = 0; i < heroesPerPlayer; i++)
                actions.add(myHeroes.get(i).move(scoutPoints[i], String.format("SCOUT %d", i)));


            // Attackers actions
            for (Monster monster : attackPriorityList)
            {
                if (attackHeroes.isEmpty())
                    break;

                Hero attacker = monster.getClosest(attackHeroes);

                actions.set(attacker.index, attacker.wait("A1"));
                attackHeroes.remove(attacker);
            }

            // Defenders actions
            for (Monster monster : defensePriorityList)
            {
                if (defenseHeroes.isEmpty())
                    break;

                Hero defender = monster.getClosest(defenseHeroes);
                
                // If monster cannot be reached (even when moving at full speed and winding ASAP)
                System.err.println(String.format("Monster at %d %d predictions:", monster.x, monster.y));
                monster.getClosestIntersection(defender);
                // if (monster.nearBase && monster.distance(myBase) <= monster.getClosestIntersection(defender) * 2 defender.distance(monster) * 2 - 1280)
                // {
                //     continue;
                // }
                // Will hit base next turn
                if (monster.distance(myBase) <= 300+400 && monster.shieldLife == 0 && myMana >= 10 && monster.distance(defender) <= 1280)
                {
                    actions.set(defender.index, defender.wind(ennemyBase, "D1"));
                    defenseHeroes.remove(defender);
                    continue;
                }
                // Ennemy might be threatening
                else if (myBase.getClosest(ennemyHeroes) != null)
                {
                    Hero closestEnnemy = myBase.getClosest(ennemyHeroes);

                    // Ennemy can wind directly into base
                    if (monster != null && monster.distance(closestEnnemy) <= 1280 && monster.distance(myBase) <= 300 + 2200
                        && myMana >= 10 && monster.shieldLife == 0 && defender.distance(monster) <= 1280)
                    {
                        actions.set(defender.index, defender.wind(ennemyBase, "D2"));
                        defenseHeroes.remove(defender);
                        continue;
                    }
                    else if (defender.distance(closestEnnemy) <= 2200 && defender.shieldLife == 0 && myMana >= 10)
                    {
                        actions.set(defender.index, defender.shield(defender, "D4"));
                        defenseHeroes.remove(defender);
                        continue;
                    }
                }
                
                // In any other case, just defend
                if (!monster.isTargeted)
                {
                    actions.set(defender.index, defender.attack(monster, monsters, "D3"));
                    defenseHeroes.remove(defender);
                    continue;
                }                
            }
            
            for (String action : actions)
            {
                System.out.println(action);
            }
        }
    }
}

