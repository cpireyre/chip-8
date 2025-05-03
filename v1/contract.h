#ifndef CONTRACT_H
# define CONTRACT_H

uint8_t		peek(Box *box, uint16_t addr);
void		poke(Box *box, uint16_t addr, uint8_t byte);
uint8_t		read(Box *box, uint8_t r);
uint8_t		load(Box *box, uint8_t r, uint8_t v);
uint16_t	look(Box *box);
void		keep(Box *box, uint16_t addr);
uint16_t	here(Box *box);
void		jump(Box *box, uint16_t addr);
void		push(Box *box, uint16_t addr);
uint16_t	grab(Box *box);
uint8_t		ping(Box *box);
void		wait(Box *box, uint8_t lag);
uint8_t		hear(Box *box);
void		sing(Box *box, uint8_t hum);

#endif /* CONTRACT_H */
