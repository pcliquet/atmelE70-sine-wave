/**
 * \file
 *
 * \brief Exemplo de Sinewave do DAC (Conversor Digital-Analógico).
 *
 * Direitos autorais (c) 2011-2018 da Microchip Technology Inc. e suas subsidiárias.
 *
 * \asf_license_start
 *
 * \page Licença
 *
 * Sujeito à sua conformidade com estes termos, você pode usar o software da Microchip
 * e quaisquer derivados exclusivamente com produtos da Microchip.
 * É sua responsabilidade cumprir os termos de licença de terceiros aplicáveis
 * ao seu uso de software de terceiros (incluindo software de código aberto) que
 * possa acompanhar o software da Microchip.
 *
 * ESTE SOFTWARE É FORNECIDO PELA MICROCHIP "COMO ESTÁ". NENHUMA GARANTIA,
 * SEJA EXPRESSA, IMPLÍCITA OU LEGAL, SE APLICA A ESTE SOFTWARE,
 * INCLUINDO QUAISQUER GARANTIAS IMPLÍCITAS DE NÃO VIOLAÇÃO, COMERCIABILIDADE,
 * E ADEQUAÇÃO A UMA FINALIDADE ESPECÍFICA. EM NENHUM CASO A MICROCHIP SERÁ
 * RESPONSÁVEL POR QUALQUER PERDA, DANO, CUSTO OU DESPESA INDIRETA, ESPECIAL,
 * PUNITIVO, INCIDENTAL OU CONSEQUENTE DE QUALQUER TIPO RELACIONADO AO
 * SOFTWARE, MESMO QUE A MICROCHIP TENHA SIDO AVISADA DA
 * POSSIBILIDADE OU OS DANOS SEJAM PREVISÍVEIS. NA EXTENSÃO MÁXIMA
 * PERMITIDA PELA LEI, A RESPONSABILIDADE TOTAL DA MICROCHIP EM TODAS AS REIVINDICAÇÕES DE QUALQUER FORMA
 * RELACIONADAS A ESTE SOFTWARE NÃO EXCEDERÁ O MONTANTE DAS TAXAS, SE HOUVER,
 * QUE VOCÊ PAGOU DIRETAMENTE À MICROCHIP POR ESTE SOFTWARE.
 *
 * \asf_license_stop
 *
 */

/**
 * \mainpage Exemplo de Sinewave do DAC (Conversor Digital-Analógico)
 *
 * \section Propósito
 *
 * O exemplo de Sinewave do DAC demonstra como usar o periférico DACC (Controlador de Conversor Digital-Analógico).
 *
 * \section Requisitos
 *
 * Este exemplo pode ser usado em placas SAM3/4/V71/E70.
 *
 * \section Descrição
 *
 * Esta aplicação tem como objetivo demonstrar como usar o DACC no modo de operação livre.
 *
 * O exemplo permite configurar a frequência e amplitude da sinewave de saída.
 * A frequência pode ser configurada de 200Hz a 3KHz, e a amplitude de pico pode ser configurada de 100 a 1023/4095 em relação à resolução de 10/12 bits.
 *
 * O exemplo também pode gerar uma onda quadrada de amplitude total como referência.
 *
 * A saída pode ser monitorada conectando o seguinte pino a um canal de um osciloscópio.
 * \copydoc dac_sinewave_example_pin_defs
 *
 * \section Uso
 *
 * -# Compile o programa e faça o download para a placa de avaliação.
 * -# No computador, abra e configure um aplicativo de terminal
 *    (por exemplo, HyperTerminal no Microsoft Windows) com estas configurações:
 *   - 115200 bauds
 *   - 8 bits de dados
 *   - Sem paridade
 *   - 1 bit de parada
 *   - Sem controle de fluxo
 * -# Na janela do terminal, o seguinte texto deve aparecer (os valores dependem
 *    da placa e chip usados):
 *    \code
      -- Exemplo de Sinewave do DAC xxx --
      -- xxxxxx-xx
      -- Compilado: xxx xx xxxx xx:xx:xx --
      -- Escolhas de Menu para este exemplo --
      -- 0: Definir frequência (200Hz-3kHz). --
      -- 1: Definir amplitude (100-4095). --
      -- i: Exibir frequência e amplitude atuais. --
      -- m: Exibir este menu. --
    \endcode
 * -# Insira comandos de acordo com o menu.
 */


/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include "asf.h"
#include "conf_board.h"
#include "conf_clock.h"
#include "conf_dacc_sinewave_example.h"

#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960


/** Analog control value */
#define DACC_ANALOG_CONTROL (DACC_ACR_IBCTLCH0(0x02) | DACC_ACR_IBCTLCH1(0x02))


/** The maximal sine wave sample data (no sign) */
#define MAX_DIGITAL   (0x7ff)
/** The maximal (peak-peak) amplitude value */
#define MAX_AMPLITUDE (DACC_MAX_DATA)
/** The minimal (peak-peak) amplitude value */
#define MIN_AMPLITUDE (100)

/** SAMPLES per cycle */
#define SAMPLES (100)

/** Default frequency */
#define DEFAULT_FREQUENCY 80
/** Min frequency */
#define MIN_FREQUENCY   80
/** Max frequency */
#define MAX_FREQUENCY   3000

/** Invalid value */
#define VAL_INVALID     0xFFFFFFFF


#define but_1 PIOD
#define but_ID_1 ID_PIOD
#define but_IDX_1 26
#define but_IDX_MASK_1 (1u << but_IDX_1)

#define but_2 PIOA
#define but_ID_2 ID_PIOA
#define but_IDX_2 24
#define but_IDX_MASK_2 (1u << but_IDX_2)

#define but_3 PIOA
#define but_ID_3 ID_PIOA
#define but_IDX_3 2
#define but_IDX_MASK_3 (1u << but_IDX_3)

#define but_4 PIOA
#define but_ID_4 ID_PIOA
#define but_IDX_4 4
#define but_IDX_MASK_4 (1u << but_IDX_4)


#define but_5 PIOA
#define but_ID_5 ID_PIOA
#define but_IDX_5 3
#define but_IDX_MASK_5 (1u << but_IDX_5)


#define LED_PIO PIOC
#define LED_PIO_ID ID_PIOC
#define LED_PIO_IDX 8
#define LED_IDX_MASK (1 << LED_PIO_IDX)

/************************************************************************/
/* VAR globais                                                          */
/************************************************************************/

volatile int b1 = 0;
volatile int b2 = 0;
volatile int b3 = 0;
volatile int b4 = 0;
volatile int b5 = 0;

/************************************************************************/
/* PROTOTYPES                                                           */
/************************************************************************/

void LED_init(int estado);
void TC_init(Tc * TC, int ID_TC, int TC_CHANNEL, int freq);
void pin_toggle(Pio *pio, uint32_t mask);

/************************************************************************/
/* Handlers                                                             */
/************************************************************************/



/************************************************************************/
/* Funcoes                                                              */
/************************************************************************/

/**
* @Brief Inicializa o pino do LED
*/

void but_callback1(void)
{

	
	if(!pio_get(but_1,PIO_INPUT, but_IDX_MASK_1 )){
		b1 = 1;		
	}
	else{
		b1 = 0;
	}
	

}

void but_callback2(void)
{

	if(!pio_get(but_2,PIO_INPUT, but_IDX_MASK_2 )){
		b2 = 1;
	}
	else{
		b2 = 0;
	}

}


void but_callback3(void)
{


	if(!pio_get(but_3,PIO_INPUT, but_IDX_MASK_3)){
		b3 = 1;
	}
	else{
		b3 = 0;
	}

}

void but_callback4(void)
{

	if(!pio_get(but_4,PIO_INPUT, but_IDX_MASK_4)){
		b4 = 1;
	}
	else{
		b4 = 0;
	}

}

void but_callback5(void)
{

	if(!pio_get(but_5,PIO_INPUT, but_IDX_MASK_5)){
		b5 = 1;
	}
	else{
		b5 = 0;
	}

}



void LED_init(int estado) {
	pmc_enable_periph_clk(LED_PIO_ID);
	pio_set_output(LED_PIO, LED_IDX_MASK, estado, 0, 0);
};



void but_init(int estado){
	pmc_enable_periph_clk(but_ID_1);
	pmc_enable_periph_clk(but_ID_2);
	pmc_enable_periph_clk(but_ID_3);
	pmc_enable_periph_clk(but_ID_4);
	pmc_enable_periph_clk(but_ID_5);

//	pio_set_input(but_1, but_IDX_MASK_1,0,0,0);
	pio_configure(but_ID_1, PIO_INPUT, but_IDX_MASK_1, PIO_PULLUP);
	pio_configure(but_ID_2, PIO_INPUT, but_IDX_MASK_2, PIO_PULLUP);
	pio_configure(but_ID_3, PIO_INPUT, but_IDX_MASK_3, PIO_PULLUP);
	pio_configure(but_ID_4, PIO_INPUT, but_IDX_MASK_4, PIO_PULLUP);
	pio_configure(but_ID_5, PIO_INPUT, but_IDX_MASK_5, PIO_PULLUP);

	
	
				  
				  
	pio_handler_set(but_1,
                  but_ID_1,
                  but_IDX_MASK_1,
                  PIO_IT_EDGE,
                  but_callback1);
				  
	pio_handler_set(but_2,
                  but_ID_2,
                  but_IDX_MASK_2,
                  PIO_IT_EDGE,
                  but_callback2);

	pio_handler_set(but_3,
                  but_ID_3,
                  but_IDX_MASK_3,
                  PIO_IT_EDGE,
                  but_callback3);
				  
	pio_handler_set(but_4,
                  but_ID_4,
                  but_IDX_MASK_4,
                  PIO_IT_EDGE,
                  but_callback4);
	
	pio_handler_set(but_5,
                  but_ID_5,
                  but_IDX_MASK_5,
                  PIO_IT_EDGE,
                 but_callback5);
				  

				  
				  

  // Ativa interrupção e limpa primeira IRQ gerada na ativacao
  pio_enable_interrupt(but_1, but_IDX_MASK_1);
  pio_get_interrupt_status(but_1);
  
  pio_enable_interrupt(but_2, but_IDX_MASK_2);
  pio_get_interrupt_status(but_2);
  
  pio_enable_interrupt(but_3, but_IDX_MASK_3);
  pio_get_interrupt_status(but_3);
  
  pio_enable_interrupt(but_4, but_IDX_MASK_4);
  pio_get_interrupt_status(but_4);
  
  pio_enable_interrupt(but_5, but_IDX_MASK_5);
  pio_get_interrupt_status(but_5);
  

  // Configura NVIC para receber interrupcoes do PIO do botao
  // com prioridade 4 (quanto mais próximo de 0 maior)
  NVIC_EnableIRQ(but_ID_1);
  NVIC_SetPriority(but_ID_1, 4);
  
  NVIC_EnableIRQ(but_ID_2);
  NVIC_SetPriority(but_ID_2, 4);
  
  NVIC_EnableIRQ(but_ID_3);
  NVIC_SetPriority(but_ID_3, 4);
  
  NVIC_EnableIRQ(but_ID_4);
  NVIC_SetPriority(but_ID_4, 4);
  
  NVIC_EnableIRQ(but_ID_5);
  NVIC_SetPriority(but_ID_5, 4);
  

}

/**
* @Brief Inverte o valor do pino 0->1/ 1->0
*/
void pin_toggle(Pio *pio, uint32_t mask) {
  if(pio_get_output_data_status(pio, mask))
    pio_clear(pio, mask);
  else
    pio_set(pio,mask);
}

/**
* Configura TimerCounter (TC) para gerar uma interrupcao no canal (ID_TC e TC_CHANNEL)
* na taxa de especificada em freq.
* O TimerCounter é meio confuso
* o uC possui 3 TCs, cada TC possui 3 canais
*	TC0 : ID_TC0, ID_TC1, ID_TC2
*	TC1 : ID_TC3, ID_TC4, ID_TC5
*	TC2 : ID_TC6, ID_TC7, ID_TC8
*
**/


#define STRING_EOL    "\r"
#define STRING_HEADER "-- DAC Sinewave Example --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL

/*! Convert wave data to DACC value
 *  Put the sinewave to an offset of max_amplitude/2.
 *  \param wave          Waveform data
 *  \param amplitude     Amplitude value
 *  \param max_digital   Maximal digital value of input data (no sign)
 *  \param max_amplitude Maximal amplitude value
 */
#define wave_to_dacc(wave, amplitude, max_digital, max_amplitude) \
	(((int)(wave) * (amplitude) / (max_digital)) + (max_amplitude / 2))

/** Current g_ul_index_sample */
uint32_t g_ul_index_sample = 0;
/** Frequency */
uint32_t g_ul_frequency = 0;
/** Amplitude */
int32_t g_l_amplitude = 0;

/** Waveform selector */
uint8_t g_uc_wave_sel = 0;

/** 100 points of sinewave samples, amplitude is MAX_DIGITAL*2 */
const int16_t gc_us_sine_data[SAMPLES] = {
	0x0,   0x080, 0x100, 0x17f, 0x1fd, 0x278, 0x2f1, 0x367, 0x3da, 0x449,
	0x4b3, 0x519, 0x579, 0x5d4, 0x629, 0x678, 0x6c0, 0x702, 0x73c, 0x76f,
	0x79b, 0x7bf, 0x7db, 0x7ef, 0x7fb, 0x7ff, 0x7fb, 0x7ef, 0x7db, 0x7bf,
	0x79b, 0x76f, 0x73c, 0x702, 0x6c0, 0x678, 0x629, 0x5d4, 0x579, 0x519,
	0x4b3, 0x449, 0x3da, 0x367, 0x2f1, 0x278, 0x1fd, 0x17f, 0x100, 0x080,

	-0x0,   -0x080, -0x100, -0x17f, -0x1fd, -0x278, -0x2f1, -0x367, -0x3da, -0x449,
	-0x4b3, -0x519, -0x579, -0x5d4, -0x629, -0x678, -0x6c0, -0x702, -0x73c, -0x76f,
	-0x79b, -0x7bf, -0x7db, -0x7ef, -0x7fb, -0x7ff, -0x7fb, -0x7ef, -0x7db, -0x7bf,
	-0x79b, -0x76f, -0x73c, -0x702, -0x6c0, -0x678, -0x629, -0x5d4, -0x579, -0x519,
	-0x4b3, -0x449, -0x3da, -0x367, -0x2f1, -0x278, -0x1fd, -0x17f, -0x100, -0x080
};



/**
 * \brief Configure console.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
#ifdef CONF_UART_CHAR_LENGTH
		.charlength = CONF_UART_CHAR_LENGTH,
#endif
		.paritytype = CONF_UART_PARITY,
#ifdef CONF_UART_STOP_BITS
		.stopbits = CONF_UART_STOP_BITS,
#endif
	};

	/* Configure console UART. */
#if !SAM4L
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
#endif
	stdio_serial_init((Usart *)CONF_UART, &uart_serial_options);
}

/**
 * \brief Get input from user, and the biggest 4-digit decimal number is allowed.
 *
 * \param ul_lower_limit The lower limit of input
 * \param ul_upper_limit The upper limit of input
 */
static uint32_t get_input_value(uint32_t ul_lower_limit,
	uint32_t ul_upper_limit)
{
	uint32_t i = 0, length = 0, value = 0;
	uint8_t uc_key, str_temp[5] = { 0 };

	while (1) {
        usart_serial_getchar((Usart *)CONSOLE_UART, &uc_key);

		if (uc_key == '\n' || uc_key == '\r') {
			puts("\r");
			break;
		}

		if ('0' <= uc_key && '9' >= uc_key) {
			printf("%c", uc_key);
			str_temp[i++] = uc_key;

			if (i >= 4) {
				break;
			}
		}
	}

	str_temp[i] = '\0';
	/* Input string length */
	length = i;
	value = 0;

	/* Convert string to integer */
	for (i = 0; i < 4; i++) {
		if (str_temp[i] != '0') {
			switch (length - i - 1) {
			case 0:
				value += (str_temp[i] - '0');
				break;

			case 1:
				value += (str_temp[i] - '0') * 10;
				break;

			case 2:
				value += (str_temp[i] - '0') * 100;
				break;

			case 3:
				value += (str_temp[i] - '0') * 1000;
				break;
			}
		}
	}

	if (value > ul_upper_limit || value < ul_lower_limit) {
		puts("\n\r-F- Input value is invalid!");
		return VAL_INVALID;
	}

	return value;
}

/**
 * \brief Display main menu.
 */
static void display_menu(void)
{
	puts("======== Menu Choices for this example ========\r");
	printf("-- 0: Set frequency(%dHz-%dkHz).\n\r",
		MIN_FREQUENCY, MAX_FREQUENCY / 1000);
	printf("-- 1: Set amplitude(%d-%d).\n\r", MIN_AMPLITUDE, MAX_AMPLITUDE);
	puts("-- i: Display present frequency and amplitude.\n\r"
			"-- w: Switch to full amplitude square wave or back.\n\r"
			"-- m: Display this menu.\n\r"
			"------------ Current configuration ------------\r");
	printf("-- DACC channel:\t%d\n\r", DACC_CHANNEL);
	printf("-- Amplitude   :\t%ld\n\r", (long)g_l_amplitude);
	printf("-- Frequency   :\t%lu\n\r", (unsigned long)g_ul_frequency);
	printf("-- Wave        :\t%s\n\r", g_uc_wave_sel ? "SQUARE" : "SINE");
	puts("===============================================\r");
}

/**
 * \brief SysTick IRQ handler.
 */
void SysTick_Handler(void)
{
	uint32_t status;
	uint32_t dac_val;
	
	status = dacc_get_interrupt_status(DACC_BASE);

	/* If ready for new data */

# if (DACC_CHANNEL == 0)
	if ((status & DACC_ISR_TXRDY0) == DACC_ISR_TXRDY0) {
#elif (DACC_CHANNEL == )1
	if ((status & DACC_ISR_TXRDY1) == DACC_ISR_TXRDY1) {
#endif

		g_ul_index_sample++;
		if (g_ul_index_sample >= SAMPLES) {
			g_ul_index_sample = 0;
		}
		dac_val = g_uc_wave_sel ?
				((g_ul_index_sample > SAMPLES / 2) ? 0 : MAX_AMPLITUDE)
				: wave_to_dacc(gc_us_sine_data[g_ul_index_sample],
					 g_l_amplitude,
					 MAX_DIGITAL * 2, MAX_AMPLITUDE);
#if !(SAMV70 || SAMV71 || SAME70 || SAMS70)
		dacc_write_conversion_data(DACC_BASE, dac_val);
#else
		dacc_write_conversion_data(DACC_BASE, dac_val, DACC_CHANNEL);
#endif
	}
}

/**
 *  \brief DAC Sinewave application entry point.
 *
 *  \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	uint8_t uc_key;
	uint32_t ul_freq, ul_amp;
	


	but_init(1);

	/* Initialize the system */
	sysclk_init();
	board_init();

	/* Initialize debug console */
	configure_console();

	/* Output example information */
	puts(STRING_HEADER);

	/* Enable clock for DACC */
#if SAM4L
sysclk_enable_peripheral_clock(DACC_BASE);
#else
sysclk_enable_peripheral_clock(DACC_ID);
#endif

	/* Reset DACC registers */
	dacc_reset(DACC_BASE);

	/* Half word transfer mode */
	dacc_set_transfer_mode(DACC_BASE, 0);

	/* Initialize timing, amplitude and frequency */
#if (SAM3N) || (SAM4L) || (SAM4N)
	/* Timing:
	 * startup                - 0x10 (17 clocks)
	 * internal trigger clock - 0x60 (96 clocks)
	 */
	dacc_set_timing(DACC_BASE, 0x10, 0x60);

	/* Enable DAC */
	dacc_enable(DACC_BASE);
#else
#if (SAM3S) || (SAM3XA)
	/* Power save:
	 * sleep mode  - 0 (disabled)
	 * fast wakeup - 0 (disabled)
	 */
	dacc_set_power_save(DACC_BASE, 0, 0);
#endif

	/* Timing:
	 * refresh        - 0x08 (1024*8 dacc clocks)
	 * max speed mode -    0 (disabled)
	 * startup time   - 0x10 (1024 dacc clocks)
	 */


	/* Enable output channel DACC_CHANNEL */
	dacc_enable_channel(DACC_BASE, DACC_CHANNEL);

	/* Set up analog current */
	dacc_set_analog_control(DACC_BASE, DACC_ANALOG_CONTROL);
#endif /* (SAM3N) */

	g_l_amplitude = MAX_AMPLITUDE / 2;
	g_ul_frequency = DEFAULT_FREQUENCY;

	//SysTick_Config(sysclk_get_cpu_hz() / (NOTE_C5 * SAMPLES));
	//SysTick_Config(sysclk_get_cpu_hz() / (440 * SAMPLES));

	/* Main menu */
	display_menu();

	while (1) {
		
/*		usart_serial_getchar((Usart *)CONSOLE_UART, &uc_key);*/
		if(b1 == 1){
			ul_freq = NOTE_C5*SAMPLES;
			SysTick_Config(sysclk_get_cpu_hz() / (ul_freq ));
			
		}
		
		if(b2 == 1){

			ul_freq = NOTE_E5;
			SysTick_Config(sysclk_get_cpu_hz() / (ul_freq * SAMPLES));
			
		}
		
		if(b3 == 1 ){

			ul_freq = NOTE_G5;
			SysTick_Config(sysclk_get_cpu_hz() / (ul_freq * SAMPLES));

		}
		
		if(b4 == 1 ){

			SysTick_Config(sysclk_get_cpu_hz() / (NOTE_A5 * SAMPLES));
		}
		
		if(b5 == 1 ){

			SysTick_Config(sysclk_get_cpu_hz() / (NOTE_B5 * SAMPLES));
		}

// 		switch (uc_key) {
// 		case '0':
// 			printf("0:\t");
// 			ul_freq = get_input_value(MIN_FREQUENCY, MAX_FREQUENCY);
// 			printf("\r\n");
// 
// 			if (ul_freq != VAL_INVALID) {
// 				printf("Set frequency to : %luHz\n\r", (unsigned long)ul_freq);
// 				SysTick_Config(sysclk_get_cpu_hz() / (ul_freq * SAMPLES));
// 				g_ul_frequency = ul_freq;
// 			}
// 			break;
// 
// 		case '1':
// 			printf("Amplitude:\t");
// 			ul_amp = get_input_value(MIN_AMPLITUDE, MAX_AMPLITUDE);
// 			printf("\r\n");
// 			if (ul_amp != VAL_INVALID) {
// 				printf("Set amplitude to : %lu\n\r", (unsigned long)ul_amp);
// 				g_l_amplitude = ul_amp;
// 			}
// 			break;
// 
// 		case 'i':
// 		case 'I':
// 			printf("-I- Frequency : %lu Hz Amwplitude : %ld\n\r",
// 				(unsigned long)g_ul_frequency, (long)g_l_amplitude);
// 			break;
// 
// 		case 'w':
// 		case 'W':
// 			printf("-I- Switch wave to : %s\n\r", g_uc_wave_sel ?
// 				"SINE" : "Full Amplitude SQUARE");
// 			g_uc_wave_sel = (g_uc_wave_sel + 1) & 1;
// 			break;
// 
// 		case 'm':
// 		case 'M':
// 			display_menu();
// 			break;
// 		}
		//puts("Press \'m\' or \'M\' to display the main menu again!\r");
	}
}
