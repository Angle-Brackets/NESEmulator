#ifndef NESEMULATOR_TITLE_H
#define NESEMULATOR_TITLE_H
#include "pixelinator.h"

typedef struct Animation {
    u32 anim_frames; //Number of frames in the animation.
    u32 anim_frame_len; //Number of frames to stay on a particular animation frame.
    u32 sheet_x; //Starting position of animation in sheet.
    u32 sheet_y; //Starting position of animation in sheet.
    u32 sheet_offset_x; //Offset in x pixels between frames of animation in sprite sheet.
    u32 sheet_offset_y; //Offset in y pixels between frames of animation in sprite sheet.
} animation_t;

typedef struct Title_Sprite {
    sprite_t* sprite;
    animation_t* animations; //Array of animations to play.
    u32 current_animation; //Current animation we're playing.
    u32 animations_len;
    u32 animation_capacity;
    u32 current_frame; //Current frame the sprite is on. Only iterated when frame timer reaches the value stated by anim_frame_len.
    u32 frame_timer; //Amount of frames that the current frame has been held on. Iterated by 1 in each call to play_animation()
} title_sprite_t;

title_sprite_t create_title_sprite(sprite_sheet* sheet, i32 x, i32 y, u32 sheet_x, u32 sheet_y, u32 width, u32 height, u32 num_animations);

/**
 * Adds a new animation to title sprite
 * @param sprite sprite to add the animation to
 * @param animation_frames number of frames in the animation
 * @param animation_frame_length length in frames of each frame of animation
 * @param sheet_x Starting sheet x position
 * @param sheet_y Starting sheet y position.
 * @param off_x Offset in x pixels between frames of animation in sprite sheet.
 * @param off_y Offset in y pixels between frames of animation in sprite sheet.
 */
void add_animation(title_sprite_t* sprite, u32 animation_frames, u32 animation_frame_length, u32 sheet_x, u32 sheet_y, u32 off_x, u32 off_y);
void play_animation(title_sprite_t* sprite, u32 animation_id);
void free_title_sprite(title_sprite_t* sprite);

#endif //NESEMULATOR_TITLE_H
