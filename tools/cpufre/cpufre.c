#include <gtk/gtk.h> 
#include <unistd.h>
#include <stdio.h>
#include <string.h>
gchar buf[200];
gchar num[200];

FILE * fd;
int ret;
pthread_t ntid;
static GtkWidget *entry1;
static GtkWidget *entry2;
static GtkWidget *entry3;
static GtkWidget *entry4;
static GtkWidget *entry5;
static GtkWidget *entry6;
static GtkWidget *entry7;
static GtkWidget *entry8;
static GtkWidget *entry9;
static GtkWidget *entry10;



//实时更新cpu频率
void cpustate_thread()
{
	gint i,j;
	//static char out[20]={0};
	//gdk_threads_enter();
	//printf("jason\n");
	
	while(1)
	{		
		//printf("hellojason\n");
		g_usleep(100000); 
		gdk_threads_enter();
		for(i=0;i<=3;i++)
		{
				if(i!=0)
				{
					//printf("hellojasonfor\n");
					sprintf(buf,"/sys/devices/system/cpu/cpu%d/online",i);
					//printf("buf = %s\n",buf);
					fd = fopen(buf,"r");
					//printf("fd = %d\n",fd);
					ret = fgets(num,2,fd);
					fclose(fd);
				}
				else
				{
					num[0]='1';
				}
				
				if(num[0]=='1')
				{
					sprintf(buf,"/sys/devices/system/cpu/cpu%d/cpufreq/scaling_cur_freq",i);
					fd = fopen(buf,"r");
					fgets(num,20,fd);
					for(j=0;num[j]!=0xA;j++)
					{
						//printf("ret = %d,num[j] = %x\n",ret,num[j]);
							
					}
					num[j]='\0';	
					fclose(fd);
					switch (i)
					{
							case 0:	gtk_entry_set_text(GTK_ENTRY(entry1),num);
									gtk_entry_set_text(GTK_ENTRY(entry5),"on");
									break;
							case 1:	gtk_entry_set_text(GTK_ENTRY(entry2),num);
									gtk_entry_set_text(GTK_ENTRY(entry6),"on");
									break;
							case 2:	gtk_entry_set_text(GTK_ENTRY(entry3),num);
									gtk_entry_set_text(GTK_ENTRY(entry7),"on");
									break;
							case 3:	gtk_entry_set_text(GTK_ENTRY(entry4),num);					
									gtk_entry_set_text(GTK_ENTRY(entry8),"on");
									break;
					}	
					
					sprintf(buf,"/sys/devices/system/cpu/cpu%d/cpufreq/scaling_governor",i);
					//printf("buf = %s\n",buf);
					fd = fopen(buf,"r");
					//printf("fd = %d\n",fd);
					ret = fgets(num,20,fd);
					fclose(fd);
					for(j=0;num[j]!=0xA;j++)
					{}
					num[j]='\0';
					gtk_entry_set_text(GTK_ENTRY(entry10),num);
					
									
				}
				else
				{
					switch (i)
					{
							case 0:gtk_entry_set_text(GTK_ENTRY(entry1),"0");
								   gtk_entry_set_text(GTK_ENTRY(entry5),"off");
								   break;
							case 1:gtk_entry_set_text(GTK_ENTRY(entry2),"0");
								   gtk_entry_set_text(GTK_ENTRY(entry6),"off");
								   break;
							case 2:gtk_entry_set_text(GTK_ENTRY(entry3),"0");
								   gtk_entry_set_text(GTK_ENTRY(entry7),"off");
								   break;
							case 3:gtk_entry_set_text(GTK_ENTRY(entry4),"0");
								   gtk_entry_set_text(GTK_ENTRY(entry8),"off");
								   break;
					}	
				}
		}
		sprintf(buf,"/sys/class/thermal/thermal_zone1/temp");
		//printf("buf = %s\n",buf);
		fd = fopen(buf,"r");
		//printf("fd = %d\n",fd);
		ret = fgets(num,8,fd);
		fclose(fd);
		num[2]='\0';
		gtk_entry_set_text(GTK_ENTRY(entry9),num);
		
		gdk_threads_leave();
	}
}

//开关cpu，cpu0不能被关闭
void PressCpustate(GtkButton *button,gpointer data)
{
	if(GPOINTER_TO_INT(data)==0)
	{
	}
	else
	{
		sprintf(buf,"/sys/devices/system/cpu/cpu%d/online",GPOINTER_TO_INT(data));
		printf("buf = %s\n",buf);
		fd = fopen(buf,"r");
		fgets(num,2,fd);
		fclose(fd);
		if(num[0]=='1')
		{
			fd = fopen(buf,"w");
			ret = fputs("0",fd);
			fclose(fd);		
		}
		else if(num[0]=='0')
		{
			fd = fopen(buf,"w");
			ret = fputs("1",fd);
			fclose(fd);	
		}
	}
}

//设置cpu的运行模式
void PressSetmode(GtkButton *button,gpointer data)
{
	int i;
	GtkWidget *combobox;
    GtkTreeModel *model;
    GtkTreeIter iter;
    gchar *text = NULL;
    combobox = (GtkWidget *)data;
    model = gtk_combo_box_get_model(GTK_COMBO_BOX(combobox));
    gtk_combo_box_get_active_iter(combobox, &iter);
    gtk_tree_model_get(model, &iter, 0, &text, -1);
    //////////////
    
    for(i=1;i<=3;i++)
	{
			if(i!=0)
			{
				sprintf(buf,"/sys/devices/system/cpu/cpu%d/online",i);
				fd = fopen(buf,"r");
				ret = fgets(num,2,fd);
				fclose(fd);
			}
			if(num[0]=='1')
			{
				sprintf(buf,"/sys/devices/system/cpu/cpu%d/cpufreq/scaling_governor",i);
				fd = fopen(buf,"w");
				fputs(text,fd);
				fclose(fd);
				
			}
			
	}
    
    //////////////
    
    
	
}
//设置下拉选项
void create_combobox_with_model(GtkWidget *combobox)
{
	int i,j,k;
	char item[10][30]={0};
    GtkListStore *store = NULL;
    GtkTreeIter iter;
    GtkCellRenderer *renderer = NULL;
	
	/////////////
	sprintf(buf,"/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_governors");
	fd = fopen(buf,"r");
	fgets(num,200,fd);
	fclose(fd);
	
	
	j=0;k=0;
	for(i=0;num[i]!=0xa;i++)
	{
		while((num[i]!=0x20)&&(num[i]!=0xa))
		{
			item[j][k]=num[i];
		
			k++;
			i++;
			
		}
		
		item[j][k]='\0';
		k=0;j++;
		if(num[i]==0xa)
		{
				i--;
		}
		
	}
	j=j-1;
	///////////////




    // 填充数据。在这里只需要一列字符串。
    store = gtk_list_store_new(1, G_TYPE_STRING);
    for(i=0;i<=j;i++)
	{
		gtk_list_store_append(store, &iter);
		gtk_list_store_set(store, &iter, 0, item[i], -1);
	}
	
	gtk_combo_box_set_model(GTK_COMBO_BOX(combobox),GTK_TREE_MODEL(store));
    // 设置GtkTreeModel中的每一项数据如何在列表框中显示
    renderer = gtk_cell_renderer_text_new();
    gtk_cell_layout_pack_start(GTK_CELL_LAYOUT(combobox), renderer, TRUE);
    gtk_cell_layout_set_attributes(
    GTK_CELL_LAYOUT(combobox), renderer, "text", 0, NULL);
}

void on_delete_event(GtkWidget *widget,GdkEvent *event,gpointer data)
{
     gtk_main_quit();
}


int main(int argc,char* argv[])
{
           /*创建指向GtkWidget类型的指针*/
		GtkWidget *window;
		GtkWidget *button;
		GtkWidget *combobox;
		GtkBuilder *builder = NULL;
            ////////////
 
            //////////
            
            /*初始化*/
		gtk_init(&argc,&argv);
		builder=gtk_builder_new();
		gtk_builder_add_from_file(builder,"cpufre.glade", NULL);
		gtk_builder_connect_signals(builder, NULL);
		gdk_threads_init(); 
           // 根据 ID 获取子构件 
           //////////////////////////////
           
           ///////////////////////////////
           
		window=GTK_WIDGET(gtk_builder_get_object(builder,"window1")); 
		if (NULL == window)
		{
			/* Print out the error. You can use GLib's message logging  */
			fprintf(stderr, "Unable to file object with id \"window\" \n");
			/* Your error handling code goes here */
		}
		entry1=GTK_ENTRY(gtk_builder_get_object(builder,"entry1"));
		entry2=GTK_ENTRY(gtk_builder_get_object(builder,"entry2"));
		entry3=GTK_ENTRY(gtk_builder_get_object(builder,"entry3"));
		entry4=GTK_ENTRY(gtk_builder_get_object(builder,"entry4"));
		entry5=GTK_ENTRY(gtk_builder_get_object(builder,"entry5"));
		entry6=GTK_ENTRY(gtk_builder_get_object(builder,"entry6"));
		entry7=GTK_ENTRY(gtk_builder_get_object(builder,"entry7"));
		entry8=GTK_ENTRY(gtk_builder_get_object(builder,"entry8"));
		entry9=GTK_ENTRY(gtk_builder_get_object(builder,"entry9"));
		entry10=GTK_ENTRY(gtk_builder_get_object(builder,"entry10"));
             
		gtk_editable_set_editable(GTK_EDITABLE(entry1),FALSE);
		gtk_editable_set_editable(GTK_EDITABLE(entry2),FALSE);
		gtk_editable_set_editable(GTK_EDITABLE(entry3),FALSE);
		gtk_editable_set_editable(GTK_EDITABLE(entry4),FALSE);
		gtk_editable_set_editable(GTK_EDITABLE(entry5),FALSE);
		gtk_editable_set_editable(GTK_EDITABLE(entry6),FALSE);
		gtk_editable_set_editable(GTK_EDITABLE(entry7),FALSE);
		gtk_editable_set_editable(GTK_EDITABLE(entry8),FALSE);
		gtk_editable_set_editable(GTK_EDITABLE(entry9),FALSE);
		gtk_editable_set_editable(GTK_EDITABLE(entry10),FALSE);
      
     
         
        combobox=GTK_COMBO_BOX(gtk_builder_get_object(builder,"combobox1"));
		create_combobox_with_model(combobox);

		button=GTK_BUTTON(gtk_builder_get_object(builder, "button1")); 
        g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(PressCpustate),(gpointer)0);
		button=GTK_BUTTON(gtk_builder_get_object(builder, "button2")); 
        g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(PressCpustate),(gpointer)1);

		button=GTK_BUTTON(gtk_builder_get_object(builder, "button3")); 
        g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(PressCpustate),(gpointer)2);

		button=GTK_BUTTON(gtk_builder_get_object(builder, "button4")); 
        g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(PressCpustate),(gpointer)3);

		button=GTK_BUTTON(gtk_builder_get_object(builder, "setmode")); 
        g_signal_connect(G_OBJECT(button),"clicked",G_CALLBACK(PressSetmode),combobox);

		g_signal_connect(G_OBJECT(window),"delete_event",G_CALLBACK(on_delete_event),NULL);
   
  
		g_object_unref(G_OBJECT(builder)); 
		g_thread_create(cpustate_thread,NULL,NULL,NULL);    
        
        gtk_widget_show_all(window);
        gdk_threads_enter();
           
				
			
        gtk_main();
        gdk_threads_leave();
        return 0;
}
