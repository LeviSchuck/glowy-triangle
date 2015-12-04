#include <cstdlib>
#include <map>

#include "defs.h"
#include "Parts/Snake.hpp"
#include "Parts/AStar.hpp"

Snake::Snake() {
    width = 0;
    height = 0;
    tryFactor = 2;
    steps = 0;
}

Snake::~Snake() {
}

bool isEmpty(AStarGrid & g, const Point & p) {
    return g.valueAt(p.x,p.y) < 1;
}

void Snake::init(Screen * sc) {
    width = sc->width;
    height = sc->height;
    tryFactor = 2;
    steps = 0;

    // Begin randomized generation
    {
        // Create the snake
        Point head;
        bool satisfied = false;
        head.x = RANDOM() % width;
        head.y = RANDOM() % height;

        snake.push_front(head);

        while(!satisfied) {
            Point tail;
            tail = head;
            switch(RANDOM() % 4) {
                case 0: tail.x--; break;
                case 1: tail.x++; break;
                case 2: tail.y--; break;
                case 3: tail.y++; break;
            }
            // Make sure the tail is not out of bounds.
            if(tail.x < 0 || tail.x >= width) continue;
            if(tail.y < 0 || tail.y >= height) continue;
            satisfied = true;

            snake.push_back(tail);
        }
    }
    {
        // Place a couple walls.

        int total = RANDOM() % 16;
        for(int i = 0; i < total; i++) {
            Point wall;
            wall.x = RANDOM() % width;
            wall.y = RANDOM() % height;
            // Ensure the walls are not where the snake is.
            if(wall.x == snake.front().x && wall.y == snake.front().y) {i--; continue;}
            if(wall.x == snake.back().x  && wall.y == snake.back().y ) {i--; continue;}
            // No point in adding walls on walls.
            if(walls.find(wall) != walls.end()) {i--; continue;}

            walls.insert(wall);
        }
    }
    
    makeFood();

}

bool Snake::step(Screen * sc) {
    bool res = stepSnake();

    // Reset all of the pixels
    for(int x = 0; x < width; x++) {
        for(int y = 0; y < height; y++){
            Pixel * px = sc->pixelAt(x,y);
            px->value = 0.0;
            px->hue = 0.0;
            px->saturation = 0.0;
        }
    }

    // Draw all walls
    for(const Point & p : walls) {
        Pixel * px = sc->pixelAt(p.x,p.y);
        px->value = 0.45;
    }

    // Draw snake
    bool isFirst = true;
    int snakeSegment = snake.size();
    int snakeSize = snake.size();
    for(const Point & p : snake) {
        Pixel * px = sc->pixelAt(p.x,p.y);

        px->value = 0.5 + (
            (float) snakeSegment
            / (float) snakeSize
            ) * 0.5;

        if(isFirst) {
            px->value = 1.0;
            isFirst = false;
        }
        snakeSegment--;
    }

    // Draw Food
    {
        Pixel * px = sc->pixelAt(food.x,food.y);
        px->hue = 60;
        px->value = 1.0;
        px->saturation = 1.0;

    }

    return res;
}

void Snake::makeFood(){
    bool satisfied = false;
    steps = 0;
    tried.clear();
    while(!satisfied) {
        food.x = RANDOM() % width;
        food.y = RANDOM() % height;

        // Make sure the food doesn't try to go on one of the walls.
        if(walls.find(food) != walls.end()) continue;

        // Make sure the food won't be placed on top of the snake
        bool foundInSnake = false;
        for(const Point & p : snake) {
            if(p.x == food.x && p.y == food.y) {
                foundInSnake = true;
                break;
            }
        }
        if(foundInSnake) continue;

        satisfied = true;
    }
}


int neighborCount(AStarGrid & g, const Point & p) {
    int c = 0;
    if(!isEmpty(g,Point(p, 0,-1))) c++;
    if(!isEmpty(g,Point(p, 1, 0))) c++;
    if(!isEmpty(g,Point(p, 0, 1))) c++;
    if(!isEmpty(g,Point(p,-1, 0))) c++;
    return c;
}





bool Snake::stepSnake() {
    AStarGrid g;
    Point head, dest;
    g.init(width,height);
    head = snake.front();
    dest = head;
    steps++;

    // set walls down
    for(const Point & p : walls) {
        g.setInfinityAt(p.x,p.y);
    }

    // Mark all but the head to be impassable too
    for(const Point & p : snake) {
        //g.setValue(p.x,p.y,snakeCost);
        g.setInfinityAt(p.x,p.y);
    }
    g.clear(head.x,head.y);

    g.setDestination(food.x,food.y);
    g.setOrigin(head.x,head.y);
    // Generate the optimal / greedy path
    while(g.step());
    while(g.pathStep());

    // Derive next step destination point.
    if     (g.isPathAt(head.x-1,head.y)) dest.x--;
    else if(g.isPathAt(head.x+1,head.y)) dest.x++;
    else if(g.isPathAt(head.x,head.y-1)) dest.y--;
    else if(g.isPathAt(head.x,head.y+1)) dest.y++;

    if(!g.isDestination(dest.x,dest.y)) {
        if( dest == head
            // If we've been here, and the next step isn't empty
            // we may be in an infinite loop
            || (tried[dest] > tryFactor)
            || !isEmpty(g,dest)
            || tryFactor > snake.size() * 5
            ) {
            // So let's try something random, up to 20 times.
            for(int i = 0; i < 20; i++) {
                dest = head;
                switch(RANDOM() % 4) {
                    case 0: dest.y--; break;
                    case 1: dest.x++; break;
                    case 2: dest.y++; break;
                    case 3: dest.x--; break;
                }
                if(isEmpty(g,dest)) break;
            }
            tryFactor++;
        }
        if(tryFactor > 100) {
            // Reset our attempts at trying
            tryFactor = 2;
            tried.clear();
        }


        // We lost
        if(dest == head) return false;
        if(!isEmpty(g,dest)) return false;
    }

    // Add to the snake
    snake.push_front(dest);

    // Did we not get food?
    if(g.isDestination(dest.x,dest.y) || steps % 100 == 99) makeFood();
    else snake.pop_back();
    tried[dest]++;
    
    return true;
}


