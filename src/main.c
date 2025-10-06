#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <signal.h>
#include <syslog.h>

static const char *BLACKLIST[] = {
    "Megabonk",
    "Rayman",
    "Overcooked",
    "Subnautica",
    "Balatro",
    NULL
};

char*
build_newdir(const char *src, const char *to_add){
	char	*result;

	result = malloc(sizeof(char) * (strlen("/proc/") + strlen(src) + strlen(to_add) + 1));
	if (result){
		 sprintf(result, "/proc/%s%s", src, to_add);
	}
	return result;
}

bool
find_game_in_cmdline(int fd, const char *game){
	char	buffer[512];

	int	bytes_read = read(fd, buffer, sizeof(buffer));
	if (bytes_read == -1)
		return false;

	if (bytes_read > 0) {
		if (bytes_read < sizeof(buffer))
			buffer[bytes_read] = '\0';
		else
			buffer[sizeof(buffer)-1] = '\0';
	}

	if (strstr(buffer, game))
		return true;
	return false;
}

int main(){
	pid_t	child = fork();
	assert(child != -1);
	if (child > 0)
		exit(EXIT_SUCCESS);
	setsid();
	child = fork();
	assert(child != -1);
	if (child > 0)
		exit(EXIT_SUCCESS);
	freopen("/tmp/gameblocker.log", "w", stdout);
    freopen("/tmp/gameblocker.log", "w", stderr);
    setvbuf(stdout, NULL, _IONBF, 0); // désactiver bufferisation
	
	while (1){
		DIR				*d;
		struct dirent	*dir;

		d = opendir("/proc");
		if (d){
			while ((dir = readdir(d)) != NULL){
				for (int i = 0; BLACKLIST[i] != NULL; i++){
					char	*tmp_path = build_newdir(dir->d_name, "/cmdline");
					if (!tmp_path)
						return (1);
					int fd = open(tmp_path, O_RDONLY);
					free(tmp_path);
					if (fd > 0){
						if (find_game_in_cmdline(fd, BLACKLIST[i])){
							kill(atoi(dir->d_name), SIGTERM);
							system("zenity --info --text='Tu devrais bosser au lieu de jouer !'");
							printf("YOUPI\n");
							fflush(stdout);
						}
						close(fd);
					}
				}
			}
			closedir(d);
			sleep(1);
		}
	}
	return (0);
}
