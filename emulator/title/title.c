#include "title.h"

title_sprite_t create_title_sprite(sprite_sheet* sheet, i32 x, i32 y, u32 sheet_x, u32 sheet_y, u32 width, u32 height, u32 num_animations){
    title_sprite_t sprite = {
            .sprite = create_sprite(x, y, width, height, sheet),
            .animations = malloc(sizeof(animation_t) * num_animations),
            .current_animation = 0,
            .animations_len = 0,
            .current_frame = 0,
            .frame_timer = 0,
            .animation_capacity = num_animations
    };

    update_sprite_from_spritesheet(sprite.sprite, sheet_x, sheet_y, 0, 0, width, height);

    return sprite;
}


void add_animation(title_sprite_t* sprite, u32 animation_frames, u32 animation_frame_length, u32 sheet_x, u32 sheet_y, u32 off_x, u32 off_y){
    if(sprite == NULL){
        WARN("Sprite was null when trying to add new animation!\n")
        return;
    }

    if(sprite->animations_len == sprite->animation_capacity){
        WARN("No remaining animation slots for sprite.\n")
        return;
    }

    sprite->animations[sprite->animations_len++] = (animation_t){
        .sheet_x = sheet_x,
        .sheet_y = sheet_y,
        .anim_frames = animation_frames,
        .anim_frame_len = animation_frame_length,
        .sheet_offset_x = off_x,
        .sheet_offset_y = off_y
    };
}

void play_animation(title_sprite_t* sprite, u32 animation_id){
    if(sprite == NULL || animation_id > sprite->animations_len){
        WARN("Null sprite or invalid animation id passed.\n")
        return;
    }


    if(animation_id != sprite->current_animation){
        //New animation swap
        sprite->current_animation = animation_id;
        sprite->current_frame = 0;
        sprite->frame_timer = 0;
    }

    //Continue animation
    update_sprite_from_spritesheet(
            sprite->sprite,
            sprite->animations[animation_id].sheet_x + (sprite->animations[animation_id].sheet_offset_x * sprite->current_frame),
            sprite->animations[animation_id].sheet_y + (sprite->animations[animation_id].sheet_offset_y * sprite->current_frame),
            0,
            0,
            sprite->sprite->width,
            sprite->sprite->height
    );


    sprite->frame_timer++;
    if(sprite->frame_timer % sprite->animations[sprite->current_animation].anim_frame_len == 0 && sprite->frame_timer > 0){
        sprite->current_frame++;
        sprite->frame_timer = 0;
    }

    sprite->current_frame %= sprite->animations[sprite->current_animation].anim_frames;
}

void free_title_sprite(title_sprite_t* sprite){
    free_sprite(sprite->sprite);
    free(sprite->animations);
    sprite->sprite = NULL;
    sprite->animations = NULL;
}