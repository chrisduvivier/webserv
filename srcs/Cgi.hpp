#ifndef CGI_HPP
# define CGI_HPP

class Cgi
{
    private:
        Cgi();
        
        //template for fork and execute the cgi birary 
        int	Cgi::execute_cgi()
        { 
            int fd[2];
            int fdin = dup(STDIN_FILENO);
            int fdout = dup(STDOUT_FILENO);
            char buffer[BUFFER_SIZE + 1];

            pipe(fd);	
            if (!fork())
            {
                dup2(fd[0], STDOUT_FILENO);
                dup2(fd[1], STDIN_FILENO);

                close(fd[0]);
                execve(SCRIPT_FILENAME, NULL, env);
                exit(0);
            }
            close(fd[1]);
            dup2(fd[0], 1);
            
            int ret = 1;
            std::string body = "";

            while(ret > 0)
            {
                memset(buffer, 0, BUFFER_SIZE);
                    ret = read(fd[0], buffer, BUFFER_SIZE);
                    body += buffer;
            }
            BODY = body;
            dup2(fdin, STDIN_FILENO);
            dup2(fdout, STDOUT_FILENO);
            
            return (ret);
        };


    private:

}

#endif