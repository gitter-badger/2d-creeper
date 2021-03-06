/* actor.c */
extern Actor actors[100];
extern int in_hero_sight(Actor *hero, Actor *a);
extern Actor *get_actor_at(Actor *actors, int x, int y);
extern void keyboard(Actor *a);
extern void tick(Actor *actors);
extern void move_relative(Actor *a, int x, int y);
extern void creeper_branch(Actor *a);
extern int check_player_near(int x, int y);
extern void Boom(Actor *a);
extern void creeper_blink(Actor *a);
extern void move_random(Actor *a);
extern Actor new_hero(int x, int y);
extern Actor new_creeper(int x, int y);
/* buffer.c */
extern void buf_screen_cls(void);
extern void buf_modify_bg_color(CHAR_INFO *b, int x, int y, int color);
extern void buf_modify_fg_color(CHAR_INFO *b, int x, int y, int color);
extern void buf_put_ch(CHAR_INFO *buf, char ch, int x, int y);
extern void buf_put_ch_with_color(CHAR_INFO *buf, int x, int y, char ch, char bg_color, char fg_color);
extern void buf_put(CHAR_INFO *buf, char *str, int x, int y);
extern void buf_put_with_color(CHAR_INFO *buf, int x, int y, char *str, char bg, char fg);
extern void buf_init(CHAR_INFO *buf, char bg, char fg);
extern void buffer_draw_coord(CHAR_INFO *buf, Actor *a, int x, int y);
extern void buffer_draw_hp(CHAR_INFO *buf, Actor *a, int x, int y);
extern void buffer_draw_frame(CHAR_INFO *buf, int x, int y, int width, int height, char color);
extern void draw(Actor *actors);
/* linux.c */
/* main.c */
extern int boundary_ok(int x, int y);
extern int wait(int ms);
extern int check_game_over(Actor *a);
extern void print_slow(char *str, int delay);
extern int main(void);
extern void game(void);
/* win.c */
extern void gotoxy(int x, int y);
extern void set_cursor(int visibility);
extern int be_input(void);
extern int get_input_2(WORD *vkey, COORD *pos);
extern void set_color(char bg, char fg);
extern void select_menu(void);
extern void buf_display(CHAR_INFO *buffer);
