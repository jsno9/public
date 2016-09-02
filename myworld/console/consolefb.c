

void fb_create(struct *con)
{
	con->id=1;
	con->init=init;
	con->uninit=uninit;
	con->write=write;
}

void init(struct console *con)
{
	signal=tcgetattr(STDIN_FILENO,&con->bak);
	con->local=con->bak;
	con->local.c_lflag&=~(ICANON|ECHO);
	tcsetattr(STDIN_FILENO,TCSANOW,&con->local);
	fcntl(0, F_SETFL, fcntl(0, F_GETFL) | O_NONBLOCK);
}

void uninit(struct console *con)
{
	con->bak.c_lflag|=(ICANON|ECHO);
	if(signal!=-1)tcsetattr(STDIN_FILENO,TCSANOW,&con->bak);
}

void write(const char *format, ...)
{
	va_list arg;

    va_start (arg, format);
	vfprintf (stdout, format, arg);
    va_end (arg);
}


