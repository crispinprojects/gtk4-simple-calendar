/***************************************************************************
 *   Author Alan Crispin                                                   *
 *   crispinalan@gmail.com                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation.                                         *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

/*
 * Use MAKEFILE to compile
 *
*/

#include <gtk/gtk.h>
#include <glib/gstdio.h>  //needed for g_mkdir

#include "customcalendar.h"

//declarations

//calendar callbks
static void callbk_calendar_day_selected(CustomCalendar *calendar, gpointer user_data);
static void callbk_calendar_next_month(CustomCalendar *calendar, gpointer user_data);
static void callbk_calendar_prev_month(CustomCalendar *calendar, gpointer user_data);
static void callbk_calendar_next_year(CustomCalendar *calendar, gpointer user_data);
static void callbk_calendar_prev_year(CustomCalendar *calendar, gpointer user_data);

static void callbk_home(GSimpleAction * action, GVariant *parameter, gpointer user_data);
static void update_label_date(CustomCalendar *calendar, gpointer user_data);

static int m_year=0;
static int m_month=0;
static int m_day=0;

static const char* m_todaycolour="lightblue";
static const char* m_eventcolour="lightpink";
static const char* m_holidaycolour="lightseagreen";
static gboolean m_frame=FALSE;

const GActionEntry app_actions[] = {   
  { "home", callbk_home}
};

//---------------------------------------------------------------------
// Update Date Label
//----------------------------------------------------------------------

static void update_label_date(CustomCalendar *calendar, gpointer user_data){

	GtkWidget *label_date = (GtkWidget *) user_data;
	//g_print("Day is : %d-%d-%d \n", m_day, m_month,m_year);
	 gchar* date_str="";
	 gchar* weekday_str="";

	 GDateTime *dt;
	 dt = g_date_time_new_local(m_year, m_month, m_day, 1, 1, 1);
	 gint day_of_week = g_date_time_get_day_of_week(dt);
	 g_date_time_unref(dt); //freeit quick

	 switch(day_of_week)
	 {
	 	case G_DATE_MONDAY:
	 		weekday_str="Monday";
	 		break;
	 	case G_DATE_TUESDAY:
	 		weekday_str="Tuesday";
	 		break;
	 	case G_DATE_WEDNESDAY:
	 		weekday_str="Wednesday";
	 		break;
	 	case G_DATE_THURSDAY:
	 		weekday_str="Thursday";
	 		break;
	 	case G_DATE_FRIDAY:
	 		weekday_str="Friday";
	 		break;
	 	case G_DATE_SATURDAY:
	 		weekday_str="Saturday";
	 		break;
	 	case G_DATE_SUNDAY:
	 		weekday_str="Sunday";
	 		break;
	 	default:
	 		weekday_str="Unknown";
	 }//switch
	
	 gchar* day_str =  g_strdup_printf("%d",m_day);
	 gchar *year_str = g_strdup_printf("%d",m_year);

	date_str =g_strconcat(date_str,weekday_str," ", day_str, " ", NULL);

	 switch(m_month)
	 {
	 	case G_DATE_JANUARY:
	 		date_str =g_strconcat(date_str,"January ",year_str, NULL);
	 		break;
	 	case G_DATE_FEBRUARY:
	 		date_str =g_strconcat(date_str,"February ",year_str, NULL);
	 		break;
	 	case G_DATE_MARCH:
	 		date_str =g_strconcat(date_str,"March ",year_str, NULL);
	 		break;
	 	case G_DATE_APRIL:
	 		date_str =g_strconcat(date_str,"April ",year_str, NULL);
	 		break;
	 	case G_DATE_MAY:
	 		date_str =g_strconcat(date_str,"May ",year_str, NULL);
	 		break;
	 	case G_DATE_JUNE:
	 		date_str =g_strconcat(date_str,"June ",year_str, NULL);
	 		break;
	 	case G_DATE_JULY:
	 		date_str =g_strconcat(date_str,"July ",year_str, NULL);
	 		break;
	 	case G_DATE_AUGUST:
	 		date_str =g_strconcat(date_str,"August ",year_str, NULL);
	 		break;
	 	case G_DATE_SEPTEMBER:
	 		date_str =g_strconcat(date_str,"September ",year_str, NULL);
	 		break;
	 	case G_DATE_OCTOBER:
	 		date_str =g_strconcat(date_str,"October ",year_str, NULL);
	 		break;
	 	case G_DATE_NOVEMBER:
	 		date_str =g_strconcat(date_str,"November ",year_str, NULL);
	 		break;
	 	case G_DATE_DECEMBER:
	 		date_str =g_strconcat(date_str,"December ",year_str, NULL);
	 		break;
	 	default:
	 		date_str =g_strconcat(date_str,"Unknown ",year_str, NULL);
	 }
	
	gtk_label_set_text(GTK_LABEL(label_date), date_str);	 
}
//----------------------------------------------------------------------
// Calendar callbks
//----------------------------------------------------------------------

static void callbk_calendar_next_month(CustomCalendar *calendar, gpointer user_data) 
{
	GtkWidget *label_date = (GtkWidget *)user_data;
	
	m_day = custom_calendar_get_day(CUSTOM_CALENDAR(calendar));
	m_month = custom_calendar_get_month(CUSTOM_CALENDAR(calendar));
	m_year = custom_calendar_get_year(CUSTOM_CALENDAR(calendar));
	//g_print("Date is : %d-%d-%d \n", m_day, m_month,m_year);
	
	custom_calendar_reset_marks(CUSTOM_CALENDAR(calendar));	
	custom_calendar_reset_holidays(CUSTOM_CALENDAR(calendar));
	//now set your marks here
	
	custom_calendar_mark_day(CUSTOM_CALENDAR(calendar), 14);
	//custom_calendar_reset_marks(CUSTOM_CALENDAR(calendar));
	custom_calendar_mark_holiday(CUSTOM_CALENDAR(calendar),25);
	
	update_label_date(CUSTOM_CALENDAR(calendar), label_date);	
	custom_calendar_update(CUSTOM_CALENDAR(calendar));	
}

static void callbk_calendar_prev_month(CustomCalendar *calendar, gpointer user_data) 
{
	GtkWidget *label_date = (GtkWidget *)user_data;
	
	m_day = custom_calendar_get_day(CUSTOM_CALENDAR(calendar));
	m_month = custom_calendar_get_month(CUSTOM_CALENDAR(calendar));
	m_year = custom_calendar_get_year(CUSTOM_CALENDAR(calendar));	
	
	//g_print("Date is : %d-%d-%d \n", m_day, m_month,m_year);
	custom_calendar_reset_marks(CUSTOM_CALENDAR(calendar));	
	custom_calendar_reset_holidays(CUSTOM_CALENDAR(calendar));
	//now set your marks here
	
	custom_calendar_mark_day(CUSTOM_CALENDAR(calendar), 14);
	//custom_calendar_reset_marks(CUSTOM_CALENDAR(calendar));
	custom_calendar_mark_holiday(CUSTOM_CALENDAR(calendar),25);
	
	update_label_date(CUSTOM_CALENDAR(calendar), label_date);	
	custom_calendar_update(CUSTOM_CALENDAR(calendar));	
}

static void callbk_calendar_next_year(CustomCalendar *calendar, gpointer user_data) 
{
	GtkWidget *label_date = (GtkWidget *)user_data;
	
	m_day = custom_calendar_get_day(CUSTOM_CALENDAR(calendar));
	m_month = custom_calendar_get_month(CUSTOM_CALENDAR(calendar));
	m_year = custom_calendar_get_year(CUSTOM_CALENDAR(calendar));
	//g_print("Date is : %d-%d-%d \n", m_day, m_month,m_year);
	
	custom_calendar_reset_marks(CUSTOM_CALENDAR(calendar));	
	custom_calendar_reset_holidays(CUSTOM_CALENDAR(calendar));
	//now set your marks here
	
	update_label_date(CUSTOM_CALENDAR(calendar), label_date);	
	custom_calendar_update(CUSTOM_CALENDAR(calendar));	
	
}

static void callbk_calendar_prev_year(CustomCalendar *calendar, gpointer user_data) 
{
	GtkWidget *label_date = (GtkWidget *)user_data;
	
	m_day = custom_calendar_get_day(CUSTOM_CALENDAR(calendar));
	m_month = custom_calendar_get_month(CUSTOM_CALENDAR(calendar));
	m_year = custom_calendar_get_year(CUSTOM_CALENDAR(calendar));		
	//g_print("Date is : %d-%d-%d \n", m_day, m_month,m_year);
	
	custom_calendar_reset_marks(CUSTOM_CALENDAR(calendar));	
	custom_calendar_reset_holidays(CUSTOM_CALENDAR(calendar));
	//now set your marks here
	
	update_label_date(CUSTOM_CALENDAR(calendar), label_date);	
	custom_calendar_update(CUSTOM_CALENDAR(calendar));	
}


static void callbk_calendar_day_selected(CustomCalendar *calendar, gpointer user_data)
{
	GtkWidget *label_date = (GtkWidget *)user_data;
	
	m_day = custom_calendar_get_day(CUSTOM_CALENDAR(calendar));
	m_month = custom_calendar_get_month(CUSTOM_CALENDAR(calendar));
	m_year = custom_calendar_get_year(CUSTOM_CALENDAR(calendar));
	//g_print("Date is : %d-%d-%d \n", m_day, m_month,m_year);
	
	//set marks here
	
	update_label_date(CUSTOM_CALENDAR(calendar), label_date);	
	custom_calendar_update(CUSTOM_CALENDAR(calendar));		
}

//----------------------------------------------------------------
// Callback home (go to current date)
//-----------------------------------------------------------------
static void callbk_home(GSimpleAction * action, GVariant *parameter, gpointer user_data)
{
	GtkWindow *window =user_data;	
	GtkWidget *calendar =g_object_get_data(G_OBJECT(window), "window-calendar-key");
	GtkWidget *label_date =g_object_get_data(G_OBJECT(window), "window-label-date-key");
	
	custom_calendar_goto_today(CUSTOM_CALENDAR(calendar));
	
	m_day = custom_calendar_get_day(CUSTOM_CALENDAR(calendar));
	m_month = custom_calendar_get_month(CUSTOM_CALENDAR(calendar));
	m_year = custom_calendar_get_year(CUSTOM_CALENDAR(calendar));
	
	//set marks here		
	
	update_label_date(CUSTOM_CALENDAR(calendar), label_date);	
	custom_calendar_update(CUSTOM_CALENDAR(calendar));		
}

//----------------------------------------------------------------------

static void startup(GtkApplication *app)
{
	 //g_print("startup  called\n");	
}


static void activate (GtkApplication *app, gpointer  user_data)
{
	GtkWidget *window;	
	GtkWidget *calendar; 
	GtkWidget *box;	
	GtkWidget *label_date; //display date
	const gchar *home_accels[2] = { "Home", NULL };
			
	// create a new window, and set its title
	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "Calendar");
	gtk_window_set_default_size(GTK_WINDOW (window),400,200);
	
	box =gtk_box_new(GTK_ORIENTATION_VERTICAL,1);
	gtk_window_set_child (GTK_WINDOW (window), box);

	//Create date label
	label_date = gtk_label_new("");
	gtk_label_set_xalign(GTK_LABEL(label_date), 0.5);
	
	//Create calendar
	calendar = custom_calendar_new();
	
	
	m_day = custom_calendar_get_day(CUSTOM_CALENDAR(calendar));
	m_month = custom_calendar_get_month(CUSTOM_CALENDAR(calendar));
	m_year = custom_calendar_get_year(CUSTOM_CALENDAR(calendar));
	//g_print("Custom Calendar Date: %d-%d-%d \n", m_day, m_month, m_year);

	update_label_date(CUSTOM_CALENDAR(calendar), label_date);
		
	g_signal_connect(CUSTOM_CALENDAR(calendar), "day-selected", G_CALLBACK(callbk_calendar_day_selected), label_date);
	g_signal_connect(CUSTOM_CALENDAR(calendar), "next-month", G_CALLBACK(callbk_calendar_next_month), label_date);
	g_signal_connect(CUSTOM_CALENDAR(calendar), "prev-month", G_CALLBACK(callbk_calendar_prev_month), label_date);
	g_signal_connect(CUSTOM_CALENDAR(calendar), "next-year", G_CALLBACK(callbk_calendar_next_year), label_date);
	g_signal_connect(CUSTOM_CALENDAR(calendar), "prev-year", G_CALLBACK(callbk_calendar_prev_year), label_date);
	
	GSimpleAction *home_action;	
	home_action=g_simple_action_new("home",NULL); //app.home
	g_action_map_add_action(G_ACTION_MAP(app), G_ACTION(home_action)); //make visible	
	g_signal_connect(home_action, "activate",  G_CALLBACK(callbk_home), window);
	
	// connect keyboard accelerator	
	gtk_application_set_accels_for_action(GTK_APPLICATION(app),"app.home", home_accels);
	
	g_object_set_data(G_OBJECT(window), "window-calendar-key",calendar);
	g_object_set_data(G_OBJECT(window), "window-label-date-key",label_date);
		
	gtk_box_append(GTK_BOX(box), label_date);
	gtk_box_append(GTK_BOX(box), calendar);
	
//======================================================================
//TESTING
//======================================================================		
	m_todaycolour="peachpuff";
	m_eventcolour ="lightsalmon";
	g_object_set(calendar, "todaycolour", m_todaycolour, NULL);
	g_object_set(calendar, "eventcolour", m_eventcolour, NULL);
	g_object_set(calendar, "holidaycolour", m_holidaycolour, NULL);	
	
	m_frame =TRUE;	
	g_object_set(calendar, "frame", m_frame, NULL);	
	
	
	custom_calendar_mark_day(CUSTOM_CALENDAR(calendar), 14);
	//custom_calendar_reset_marks(CUSTOM_CALENDAR(calendar));
	custom_calendar_mark_holiday(CUSTOM_CALENDAR(calendar),25);
	//custom_calendar_reset_holidays(CUSTOM_CALENDAR(calendar));

//======================================================================
		
	custom_calendar_goto_today(CUSTOM_CALENDAR(calendar));
	custom_calendar_update(CUSTOM_CALENDAR(calendar));
	
	gtk_window_present (GTK_WINDOW (window));  //use present with gtk4 
	
}

int main (int  argc, char **argv)
{		
	GtkApplication *app;
	int status;
	app = gtk_application_new ("org.gtk.calendar", G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect_swapped(app, "startup", G_CALLBACK (startup),app);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);
	return status;
}
