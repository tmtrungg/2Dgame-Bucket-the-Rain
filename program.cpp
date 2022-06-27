#include "splashkit.h"
#include <string>
using namespace std;


void load_resources()
{
    load_resource_bundle("game_bundle", "rain_catcher.txt");
}

//make new bucket using enum if needed
;
enum rain_kind
{
    BLUE,
    PURP
};
enum buckettype
{
    BASIC=1,
    WOODEN=2,
    GOLD=3
};

//draw the rains
bitmap rain_bitmap(rain_kind kind)
{
    switch (kind)
    {
    case BLUE:
        return bitmap_named("rain");
        break;
    case PURP:
        return bitmap_named("purple");
    
        break;
    default :
        break;
    }
}

//draw different bucket
bitmap bucket_bitmap(buckettype type)
{
    switch (type)
    {
    case BASIC:
        return bitmap_named("bucket");
        break;
    case WOODEN:
        return bitmap_named("bucket2");
        break;
    case GOLD:
        return bitmap_named("bucket3");
        break;
    default :
        break;
    }
}

//player strut ( the bucket )
struct player
{
    sprite player_sprite;
    
    buckettype type_of_bucket;
};

//the rain struct
struct rain
{
    sprite rain_pic;
    rain_kind rain_type;
};

//game struct contain rain and player
struct game_info
{
    player bucket;
    vector <rain> rain_object;
    int score;
};

//create a new player
player new_player()
{
    player result;

    bitmap default_bitmap = bucket_bitmap(GOLD);        //set default bitmap of bucket is the gold one

    draw_bitmap(default_bitmap,0,0);

    result.player_sprite = create_sprite(default_bitmap);

    //set the bitmap and the middle down of the screen ( which x = 400 and y = 800)
    sprite_set_x(result.player_sprite,
    (screen_width() - sprite_width(result.player_sprite)) / 2);
    sprite_set_y(result.player_sprite,
    (screen_height() - sprite_height(result.player_sprite)));

    return result;
}

//update the player when user interact
void update_player(player &player_to_up)
{
    //move left
    if (key_down(LEFT_KEY))
    {
        sprite_set_x(player_to_up.player_sprite,
        sprite_x(player_to_up.player_sprite) - 15);
    }
    //move right
    if (key_down(RIGHT_KEY))
    {
        sprite_set_x(player_to_up.player_sprite,
        sprite_x(player_to_up.player_sprite) + 15);
    }
}

//draw the player sprite
void draw_player(player &to_draw)
{
    draw_sprite(to_draw.player_sprite);

}

//create the new rain
rain new_rain(double x, double y)
{
    rain result;
    rain_kind kind;
    kind = static_cast<rain_kind>(rnd(2));                  //random type between blue and purple
    result.rain_pic = create_sprite(rain_bitmap(kind));
    result.rain_type = kind;
    sprite_set_x(result.rain_pic, x);                       //set sprite x
    sprite_set_y(result.rain_pic, y);                       //set sprite y
    sprite_set_dy(result.rain_pic,rnd(5,20));               //set the falling speed of the rain between 5 and 20
    return result;
}

//draw out the rain sprite
void draw_rain(const rain &rain)
{
    draw_sprite(rain.rain_pic);
}

//adding new rain to the vector of the game
void add_rain(game_info &game)
{
    double x,y;

    //Random x and y position for rain drop
    x = rnd(0,800);         //random x the rain appear   
    y = -50;

    //
    game.rain_object.push_back(new_rain(x,y));
}

//update the rain sprite
void update_rain(rain &rain)
{
    double x_rain, y_rain;
    x_rain = sprite_x(rain.rain_pic);
    y_rain = sprite_y(rain.rain_pic);
    update_sprite(rain.rain_pic);
}

//apply the change to the game when the rain touch the bucket
void apply_rain(game_info &game, int ri)
{
    rain_kind kind;
    kind = game.rain_object[ri].rain_type; // rain index
    switch(kind)
    {
        case BLUE:              //touch the blue rain
            game.score = game.score + 1;
            write(game.score);
            break;
        case PURP:              //touch the purple rain
            //if the player touch the purple rain and not lose yet, he will get a dealy of 0.5s to know that his score is minus.
            if (game.score - 10 > 0)        
            {
                delay(500);
                game.score = game.score - 10;
            }
            //if the player touch the purple rain and lose, no delay will be applied
            if (game.score - 10 < 0)
            {
                game.score = game.score - 10;
            }
            break;
        default:
            break;

    }

}

//remove the rain when it touch the bucket
void remove_rain(game_info &game,int ri)
{
    game.rain_object[ri] = game.rain_object[game.rain_object.size()-1];
    game.rain_object.pop_back();
}

//check if the bucket can get any rain and do the action
void check_collision(game_info &game)
{
    sprite player_sprite = game.bucket.player_sprite;
    sprite rain_pic;
    int rain_number = game.rain_object.size() - 1;
    for ( rain_number; rain_number >= 0 ; rain_number-- )
    {
        rain_pic = game.rain_object[rain_number].rain_pic;
        if (sprite_collision(player_sprite,rain_pic))
        {
          apply_rain(game,rain_number);
          remove_rain(game,rain_number); 
        }
    }
}

//update the game
void update_game(game_info &game)
{
    update_player(game.bucket);
    for (int i = 0 ; i < game.rain_object.size() ; i++ )
    {
        update_rain(game.rain_object[i]);
    }
    //EASY MODE ( SCORE < 10 )
    if (game.score < 10)
    {
        if (rnd()<0.15)         //controlling the amount of rain falling in 1 updateS
            {
                add_rain(game);
            }
    }
    //MEDIUM MODE ( SCORE > 10 AND < 20 )
    if (game.score >10 and game.score <20)
    {
        if (rnd()<0.20)         //controlling the amount of rain falling in 1 updateS
            {
                add_rain(game);
            }
    } 
    //MASTER MODE ( SCORE > 20 )
    if (game.score >20)
    {
        if (rnd()<0.25)         //controlling the amount of rain falling in 1 updateS
            {
                add_rain(game);
            }
    } 
    check_collision(game);
}

//draw the game after 1 update
void draw_game(game_info &game)
{
    //clear_screen(COLOR_BLACK);
    draw_bitmap("bg",0,0);          //background
    draw_text("SCORE: " + to_string(game.score), COLOR_BLACK,350, 30);      //score in the middle
    draw_player(game.bucket);
    for (int i = 0; i < game.rain_object.size(); i++)
    {
        draw_rain(game.rain_object[i]);
    }
    if (game.score < 0)
        {   
            refresh_screen(60);
            draw_bitmap("over",90,300);
            refresh_screen(60);
            delay(2500);
            exit(0);
        }
    refresh_screen(60);

}
int main()
{
    open_window("Rain Catcher", 800, 800);
    load_resources();
    game_info game;
    game.bucket = new_player();
    game.score = 0;
    draw_bitmap("rule",0,0);
    refresh_screen(60);
    delay(10000);

    while (not quit_requested())
    {
        process_events();
        update_game(game);
        draw_game(game);
        
        refresh_screen(60);
    }

    return 0;
}
